#!/bin/bash

# Phoebus build and installation location
TOP="/usr/local/epics/phoebus-fnal"
EPICS_CONFIG="/usr/local/epics/Config"

#export JAVA_HOME=/usr/local/jdk-13.0.2
export JAVA_HOME=/usr/lib/jvm/jre-openjdk/
export PATH="$JAVA_HOME/bin:$PATH"
#export EPICS_CA_ADDR_LIST="${EPICS_CA_ADDR_LIST} 131.225.120.160:6721 131.225.120.160:6722 131.225.120.160:6723" Temporary
export EPICS_CA_ADDR_LIST="${EPICS_CA_ADDR_LIST} 131.225.120.160:6721 131.225.120.160:6722 131.225.120.160:6723 131.225.120.164"

echo $TOP
#V="4.6.6-SNAPSHOT"
#V="4.7.2-SNAPSHOT"
V="4.7.3-SNAPSHOT"

# figure out the path to the product jar
if [[ -z "${PHOEBUS_JAR}" ]]; then
  PHOEBUS_JAR=${TOP}/product-fnal/target/fnal-product-${V}.jar
fi

# figure out the path to the configuration settings
if [[ -z "${PHOEBUS_CONFIG}" ]]; then
    PHOEBUS_CONFIG=${EPICS_CONFIG}/CSS/Phoebus/settings.ini
fi
PIP2IT_LAYOUT=${EPICS_CONFIG}/CSS/Phoebus/pip2it.memento
echo $PHOEBUS_JAR

# To get one instance, use server mode
ID=$(id -u)
#OPT="-server 4$ID"
OPT=""

JDK_JAVA_OPTIONS=" -DCA_DISABLE_REPEATER=true"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dnashorn.args=--no-deprecation-warning"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Djdk.gtk.verbose=false -Djdk.gtk.version=2 -Dprism.forceGPU=true"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dlogback.configurationFile=/home/train/epics-tools/setup/settings/logback.xml"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dorg.csstudio.javafx.rtplot.update_counter=false"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dfile.encoding=UTF-8"
export JDK_JAVA_OPTIONS

echo $JDK_JAVA_OPTIONS

java -jar $PHOEBUS_JAR -settings $PHOEBUS_CONFIG -layout $PIP2IT_LAYOUT -logging $TOP/config/logging.properties $OPT "$@" &
