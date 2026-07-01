#!/usr/bin/env bash

# Start phoebus using ACQ400CSSP helper script

ROOT_DIR=$(dirname "$(realpath "$0")")

# Get UUT
UUT=$1
if [ -z "$UUT" ]; then
    echo -e "\n[ACQ400CSSP]"
    read -p "Connect to UUT : " UUT
fi
if [ ${#UUT} -lt 8 ]; then
    echo "Invalid UUT"
    exit 1
fi

# CHANGE ME
PHOEBUS_JAR="$ROOT_DIR/product-4.7.3/product-4.7.3.jar"

# Vars
JAVA_EXE="java -Dlog.level=DEBUG"
SETTINGS_BASE="$ROOT_DIR/src/settings_base.ini"
LAUNCHER="$ROOT_DIR/src/acq400_launcher.bob"
RESOURCE="${LAUNCHER}"
WORKSPACE="$ROOT_DIR/workspaces/$UUT"
JAVA_PREFS="-Dphoebus.user=${WORKSPACE} -Dphoebus.folder.name.preference= "
SETTINGS="$WORKSPACE/settings.ini"
MEMENTO="$WORKSPACE/memento"

# Attempt to locate a phoebus jar if not found at PHOEBUS_JAR
if ! [ -f "$PHOEBUS_JAR" ]; then
    shopt -s nullglob
    PHOEBUS_JAR=($ROOT_DIR/product-*/*.jar)
    if [ ${#PHOEBUS_JAR[@]} -eq 0 ]; then
        echo "Error: Unable to locate phoebus jar"
        exit 1
    fi
    PHOEBUS_JAR="${PHOEBUS_JAR[0]}"
    echo "Found phoebus jar ${PHOEBUS_JAR}"
fi

# If ioc_addr.conf or acq2106_000_addr.conf files exist use contents for ADDR_LIST
ADDR_LIST=""
[ -f "${ROOT_DIR}/ioc_addr.conf" ] && ADDR_LIST=$(cat "${ROOT_DIR}/ioc_addr.conf")
[ -f "${ROOT_DIR}/${UUT}_addr.conf" ] && ADDR_LIST=$(cat "${ROOT_DIR}/${UUT}_addr.conf")

if [ -n "$ADDR_LIST" ]; then
    JAVA_PREFS="${JAVA_PREFS} -Djca.use_env=true"
    export EPICS_PVA_ADDR_LIST="${ADDR_LIST}"
    export EPICS_CA_ADDR_LIST="${ADDR_LIST}"
    export EPICS_CA_MAX_ARRAY_BYTES=500000
    echo "Using addr list: ${ADDR_LIST}"
fi

# Init workspace if needed
if [ ! -f "$MEMENTO" ]; then
    echo "Creating new workspace $WORKSPACE"
    mkdir -p $WORKSPACE
    cp $SETTINGS_BASE $SETTINGS
    echo "org.csstudio.display.builder.model/macros=<UUT>${UUT}</UUT>" >> $SETTINGS
    TARGET="-resource $RESOURCE -layout null"
else
    echo "Using existing workspace $WORKSPACE"
    TARGET="-layout $MEMENTO"
fi

# Run
CMD="$JAVA_EXE $JAVA_PREFS -jar $PHOEBUS_JAR -nosplash -settings $SETTINGS $TARGET"
echo "Running cmd: $CMD"
$CMD &> /dev/null
echo "Done"
