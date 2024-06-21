#!../../bin/linux-x86_64/ina-management

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

cd "${TOP}/iocBoot/${IOC}"
iocInit

