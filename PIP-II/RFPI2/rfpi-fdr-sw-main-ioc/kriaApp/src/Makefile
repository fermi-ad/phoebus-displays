TOP=../..
FW_PATH=/home/ubuntu/rfpi-fw/zynq/apps
#FW_PATH=/home/kacper/new-poc/rfpi-fw/zynq/apps

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = kria
# kria.dbd will be created and installed
DBD += kria.dbd

# zynq.dbd will be made up from these files:
kria_DBD += base.dbd
kria_DBD += asyn.dbd
kria_DBD += iocAdmin.dbd
kria_DBD += kriaAsynPortDriver.dbd
kria_DBD += asSupport.dbd

kria_DBD += PVAServerRegister.dbd
kria_DBD += qsrv.dbd

USR_INCLUDES += -I$(FW_PATH)/src/includes

# Include dbd files from all support applications:
#kria_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
kria_LIBS += asyn
kria_LIBS += autosave
kria_LIBS += devIocStats
kria_LIBS += qsrv
kria_LIBS += pvAccessIOC pvAccessCA pvAccess pvData
kria_LIBS += tools core

tools_DIR += $(FW_PATH)/lib 
core_DIR += $(FW_PATH)/lib 

# kria_registerRecordDeviceDriver.cpp derives from kria.dbd
kria_SRCS += kria_registerRecordDeviceDriver.cpp
kria_SRCS += kriaAsynPortDriver.cpp 
kria_SRCS += kriaParamsReadout.cpp
kria_SRCS += kriaReg.cpp 
# Build the main IOC entry point on workstation OSs.
kria_SRCS_DEFAULT += kriaMain.cpp
kria_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#kria_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
kria_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

