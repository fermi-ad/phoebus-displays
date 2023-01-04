#!/bin/bash

ALARMS=/usr/local/phoebus-fnal/lib/phoebus/services/alarm-server

cd $ALARMS
./alarm-server.sh -config CMTF -server acsys-services.fnal.gov:9092 -import ~/epicsDEV/Config/CSS/Phoebus/Alarms/cmtf.xml -settings /usr/local/epics/Config/CSS/Phoebus/settings.ini

./alarm-server-cmtf.sh
