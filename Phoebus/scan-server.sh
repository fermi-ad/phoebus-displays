#!/bin/sh
#
# Scan Server launcher for Linux or Mac OS X

# When deploying, change "TOP"
# to the absolute installation path
TOP="/usr/local/epics/phoebus-fnal/lib/phoebus/services/scan-server"

# Ideally, assert that Java is found
export JAVA_HOME=/usr/local/jdk-13.0.2
export PATH="$JAVA_HOME/bin:$PATH"

if [ -d $TOP/target ]
then
  TOP="$TOP/target"
fi

JAR=`echo "${TOP}/service-scan-server-*.jar"`
java -jar $JAR "$@"
