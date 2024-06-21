TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ina-management
# ina-management.dbd will be created and installed
DBD += ina-management.dbd

# ina-management.dbd will be made up from these files:
ina-management_DBD += base.dbd
ina-management_DBD += asyn.dbd
ina-management_DBD += inaAsynPortDriver.dbd
ina-management_DBD += asSupport.dbd

ina-management_DBD += PVAServerRegister.dbd
ina-management_DBD += qsrv.dbd

USR_INCLUDES += -I$(FW_PATH)/includes -I$(GPIO)/include

# Include dbd files from all support applications:
#ina-management_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ina-management_LIBS += xxx
ina-management_LIBS += asyn
ina-management_LIBS += qsrv
ina-management_LIBS += pvAccessIOC pvAccessCA pvAccess pvData
ina-management_LIBS += autosave

ina-management_LIBS += rfpimgmt rfpigpio rfpii2c
rfpimgmt_DIR=$(FW_PATH)/lib 
rfpigpio_DIR=$(FW_PATH)/lib 
rfpii2c_DIR=$(FW_PATH)/lib 

# ina-management_registerRecordDeviceDriver.cpp derives from ina-management.dbd
ina-management_SRCS += ina-management_registerRecordDeviceDriver.cpp
ina-management_SRCS += inaAsynPortDriver.cpp 
ina-management_SRCS += inaParamsReadout.cpp 
ina-management_SRCS += inaReg.cpp 

# Build the main IOC entry point on workstation OSs.
ina-management_SRCS_DEFAULT += ina-managementMain.cpp
ina-management_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ina-management_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ina-management_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

