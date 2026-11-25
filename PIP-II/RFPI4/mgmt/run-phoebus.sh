#!/bin/sh
# Start Phoebus (macOS app bundle) with the RFPI panel and opi/system.ini.
#
# Phoebus 4.7 reads its bundled settings.ini from the current directory, so it has to be
# started from its install directory ("Unable to locate settings file: settings.ini").
#
#   opi/run-phoebus.sh [panel.bob]          default: RFPI_main.bob
#   PHOEBUS_APP=/path/CSS_Phoebus.app opi/run-phoebus.sh
set -e
OPI=$(cd "$(dirname "$0")" && pwd)

for app in "$PHOEBUS_APP" "$HOME/Desktop/EPICS/CSS_Phoebus.app" /Applications/CSS_Phoebus.app; do
    [ -n "$app" ] && [ -e "$app/Contents/MacOS/main.command" ] && break
    app=
done
[ -n "$app" ] || { echo "Phoebus not found - set PHOEBUS_APP=/path/CSS_Phoebus.app" >&2; exit 1; }
[ -f "$OPI/system.ini" ] || { echo "$OPI/system.ini missing (written by the build, see README)" >&2; exit 1; }

install=$(ls -d "$app"/phoebus-* 2>/dev/null | head -n 1)
[ -n "$install" ] || { echo "no phoebus-* directory in $app" >&2; exit 1; }

cd "$install"
exec sh "$app/Contents/MacOS/main.command" -settings "$OPI/system.ini" -resource "$OPI/${1:-RFPI_main.bob}"
