#!../../bin/linux-arm/ina-management

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/ina-management.dbd"
ina_management_registerRecordDeviceDriver pdbbase

epicsEnvSet("acc",   "PIP2IT")
epicsEnvSet("sys",   "pHB650")
epicsEnvSet("group", "RFPI")
epicsEnvSet("num",   "ina")
epicsEnvSet("PORT",  "port")
epicsEnvSet("CONFIG_PATH", "/home/kacper/new-ioc/rfpi-fdr-sw-mgmt-lib/CLI/config.json")

inaAsynPortDriverConfigure("$(PORT)","$(CONFIG_PATH)")

## Load record instances
dbLoadRecords("db/slot.db",	"PORT=$(PORT),ADDR=0")
dbLoadRecords("db/fan.db",	"PORT=$(PORT),ADDR=0")
dbLoadRecords("db/general.db",  "PORT=$(PORT),ADDR=0")
dbLoadRecords("db/circuit.db",  "PORT=$(PORT),ADDR=0")

# autosave config
set_savefile_path("/home/epics/autosave/management-ioc")
set_requestfile_path("${TOP}/iocBoot/${IOC}")
set_pass1_restoreFile("setpoints.sav", "acc=$(acc),sys=$(sys),group=$(group)")
save_restoreSet_DatedBackupFiles(1)


cd "${TOP}/iocBoot/${IOC}"
iocInit

create_monitor_set("setpoints.req", 1, "acc=$(acc),sys=$(sys),group=$(group)")
