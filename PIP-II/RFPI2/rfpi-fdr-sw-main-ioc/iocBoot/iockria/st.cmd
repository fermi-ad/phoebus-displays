#!../../bin/linux-aarch64/kria

#- You may have to change kria to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/kria.dbd"
kria_registerRecordDeviceDriver pdbbase


epicsEnvSet("acc",   "PIP2IT")
epicsEnvSet("sys",   "pHB650")
epicsEnvSet("group", "RFPI")
epicsEnvSet("num",   "kria")
epicsEnvSet("PORT",  "port")

kriaAsynPortDriverConfigure("$(PORT)")

## Load record instances
dbLoadRecords("db/analog_input.db","ADDR=0,PORT=$(PORT)")
dbLoadRecords("db/binary_input.db","ADDR=0,PORT=$(PORT)")
dbLoadRecords("db/output.db",	   "ADDR=0,PORT=$(PORT)")
dbLoadRecords("db/expert.db",	   "ADDR=0,PORT=$(PORT)")
dbLoadRecords("db/reset.db",	   "ADDR=0,PORT=$(PORT)")

dbLoadRecords("$(IOCSTATS)/db/iocAdminSoft.db", "IOC=$(acc):$(sys)_$(group)_:")

# autosave config
set_savefile_path("/opt/iocs/autosave/kria-ioc")
set_requestfile_path("${TOP}/iocBoot/${IOC}")
set_pass1_restoreFile("setpoints.sav", "acc=$(acc),sys=$(sys),group=$(group)")
save_restoreSet_DatedBackupFiles(1)


cd "${TOP}/iocBoot/${IOC}"
iocInit

create_monitor_set("setpoints.req", 1, "acc=$(acc),sys=$(sys),group=$(group)")
