TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ADGStreamer
PROD_NAME = $(PROD_IOC)
# ADGStreamer.dbd will be created and installed
DBD += ADGStreamer.dbd

# ADGStreamer.dbd will be made up from these files:
ADGStreamer_DBD += base.dbd

# Include dbd files from all support applications:
#ADGStreamer_DBD += xxx.dbd
ADGStreamer_DBD += ADGStreamer.dbd

# Add all the support libraries needed by this IOC
#ADGStreamer_LIBS += xxx
ADGStreamer_LIBS += ADGStreamer

# ADGStreamer_registerRecordDeviceDriver.cpp derives from ADGStreamer.dbd
ADGStreamer_SRCS += ADGStreamer_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ADGStreamer_SRCS_DEFAULT += ADGStreamerMain.cpp
ADGStreamer_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ADGStreamer_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ADGStreamer_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(ADCORE)/ADApp/commonDriverMakefile

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

