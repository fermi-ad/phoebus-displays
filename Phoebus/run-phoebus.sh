#!/bin/bash

# Phoebus build and installation location
TOP="/usr/local"
#EPICS_CONFIG="/usr/local/epics/Config"
EPICS_CONFIG="/home/epics/epicsDEV/Config"

#export JAVA_HOME=$TOP/lib/jvm/jdk-11.0.2
export JAVA_HOME=/usr/local/jdk-13.0.2
export PATH="$JAVA_HOME/bin:$PATH"

echo $TOP
V="4.6.6-SNAPSHOT"

# figure out the path to the product jar
if [[ -z "${PHOEBUS_JAR}" ]]; then
  PHOEBUS_JAR=${TOP}/phoebus-fnal/products/target/fnal-product-${V}.jar
fi

# figure out the path to the configuration settings
if [[ -z "${PHOEBUS_CONFIG}" ]]; then
#  PHOEBUS_CONFIG=${TOP}/phoebus-fnal/config/settings.ini
    PHOEBUS_CONFIG=${EPICS_CONFIG}/CSS/Phoebus/settings.ini
fi

# To get one instance, use server mode
ID=$(id -u)
OPT="-server 4$ID"

JDK_JAVA_OPTIONS=" -DCA_DISABLE_REPEATER=true"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dnashorn.args=--no-deprecation-warning"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Djdk.gtk.verbose=false -Djdk.gtk.version=2 -Dprism.forceGPU=true"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dlogback.configurationFile=/home/train/epics-tools/setup/settings/logback.xml"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dorg.csstudio.javafx.rtplot.update_counter=false"
JDK_JAVA_OPTIONS="$JDK_JAVA_OPTIONS -Dfile.encoding=UTF-8"
export JDK_JAVA_OPTIONS

echo $JDK_JAVA_OPTIONS

java -jar $PHOEBUS_JAR -settings $PHOEBUS_CONFIG -logging $TOP/phoebus-fnal/config/logging.properties $OPT "$@" &
