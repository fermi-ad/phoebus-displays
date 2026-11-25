#!/usr/bin/env bash
# Starts Phoebus with the displays of the RFPI logic unit (kria IOC):
# PV naming macros from system.ini next to this script (made by the IOC build
# for its SYSTEM, see iocBoot/iockria/Makefile) plus Channel Access settings
# for the IOC address, then opens RFPI_kria_main.bob.
#
# Usage: run-rfpi-logic-opi.sh [-a IOC_ADDR] [-p PHOEBUS] [-s SYSTEM_INI] [-d DISPLAY.bob] [-- phoebus options]
#   -a IOC_ADDR    address(es) of the IOC for EPICS_CA_ADDR_LIST, space separated
#                  (default: $RFPI_IOC_ADDR or 10.1.6.2)
#   -p PHOEBUS     Phoebus to run: its directory, phoebus.sh or the product .jar
#                  (default: $PHOEBUS, phoebus/phoebus.sh in PATH, ~/phoebus,
#                  /opt/phoebus, /Applications/phoebus*)
#   -s SYSTEM_INI  PV naming macros of the system (default: $RFPI_SYSTEM_INI or
#                  system.ini next to this script)
#   -d DISPLAY     display to open (default: RFPI_kria_main.bob next to this script)
set -euo pipefail

HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
IOC_ADDR=${RFPI_IOC_ADDR:-10.1.6.2}
PHOEBUS=${PHOEBUS:-}
DISPLAY_FILE=$HERE/RFPI_kria_main.bob
SYSTEM_INI=${RFPI_SYSTEM_INI:-$HERE/system.ini}

die() { echo "run-rfpi-logic-opi: $*" >&2; exit 1; }
usage() { sed -n '2,15p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit "${1:-0}"; }

while getopts "a:p:s:d:h" opt; do
    case $opt in
        a) IOC_ADDR=$OPTARG ;;
        p) PHOEBUS=$OPTARG ;;
        s) SYSTEM_INI=$OPTARG ;;
        d) DISPLAY_FILE=$OPTARG ;;
        h) usage 0 ;;
        *) usage 1 ;;
    esac
done
shift $((OPTIND - 1))

[ -f "$SYSTEM_INI" ] || die "$SYSTEM_INI not found -- give it with -s or build the IOC (iocBoot/iockria: make) to generate it"
[ -f "$DISPLAY_FILE" ] || die "display $DISPLAY_FILE not found"
DISPLAY_FILE=$(cd "$(dirname "$DISPLAY_FILE")" && pwd)/$(basename "$DISPLAY_FILE")

# Phoebus launcher: phoebus.sh of a product directory, or java -jar on its jar
launcher=()
phoebus_dir=
pick() {
    local p=$1 jar
    # absolute: the launcher is started from the installation directory
    if [ -d "$p" ]; then p=$(cd "$p" && pwd)
    elif [ -e "$p" ]; then p=$(cd "$(dirname "$p")" && pwd)/$(basename "$p")
    else return 1
    fi
    if [ -d "$p" ]; then
        phoebus_dir=$p
        if [ -x "$p/phoebus.sh" ]; then launcher=("$p/phoebus.sh"); return 0; fi
        jar=$(ls "$p"/product*.jar 2>/dev/null | head -1 || true)
        if [ -n "$jar" ]; then launcher=(java -jar "$jar"); return 0; fi
        return 1
    fi
    phoebus_dir=$(dirname "$p")
    case $p in
        *.jar) [ -f "$p" ] && launcher=(java -jar "$p") && return 0 ;;
        *)     [ -x "$p" ] && launcher=("$p") && return 0 ;;
    esac
    return 1
}
if [ -n "$PHOEBUS" ]; then
    pick "$PHOEBUS" || die "no Phoebus at $PHOEBUS (directory with phoebus.sh or product*.jar, phoebus.sh, or the .jar)"
else
    found=0
    for c in "$(command -v phoebus 2>/dev/null || true)" "$(command -v phoebus.sh 2>/dev/null || true)" \
             "$HOME/phoebus" /opt/phoebus /Applications/phoebus*; do
        [ -n "$c" ] && [ -e "$c" ] && pick "$c" && { found=1; break; }
    done
    [ "$found" = 1 ] || die "Phoebus not found -- give it with -p or PHOEBUS=<directory|phoebus.sh|product.jar>"
fi

# Settings: system.ini (PV naming macros) + Channel Access to the IOC.
# Kept (overwritten on every run), not a temporary file: phoebus.sh starts
# the UI in the background and returns at once, Phoebus reads the file later.
# The name needs the .ini extension (Phoebus reads .xml as XML).
settings=${RFPI_OPI_SETTINGS:-$HOME/.phoebus/rfpi-logic-opi-settings.ini}
mkdir -p "$(dirname "$settings")"
settings=$(cd "$(dirname "$settings")" && pwd)/$(basename "$settings")
{
    cat "$SYSTEM_INI"
    echo "org.phoebus.pv/default=ca"
    echo "org.phoebus.pv.ca/addr_list=$IOC_ADDR"
    echo "org.phoebus.pv.ca/auto_addr_list=false"
} > "$settings"

echo "Phoebus: ${launcher[*]}"
echo "IOC (CA addr_list): $IOC_ADDR"
echo "Macros: $(grep -o 'macros=.*' "$SYSTEM_INI" || echo '(none)')"
echo "Display: $DISPLAY_FILE"
echo "System: $SYSTEM_INI"
echo "Settings: $settings"
# phoebus.sh adds "-server 4918": with Phoebus already running, the display
# opens in that instance, which keeps its own settings (IOC address, macros).
if command -v nc >/dev/null 2>&1 && nc -z localhost 4918 2>/dev/null; then
    echo "Note: a Phoebus is already running (port 4918) -- the display opens there with its settings;"
    echo "      close it first for these settings to apply."
fi
# Run from the Phoebus installation directory: its launcher loads the bundled
# <install>/settings.ini by its bare name ("settings.ini"), relative to the
# current directory (seen in 4.7.4: "Unable to locate settings file:
# settings.ini" when started elsewhere). All paths given here are absolute.
cd "$phoebus_dir"
"${launcher[@]}" -settings "$settings" -resource "$DISPLAY_FILE" "$@"
