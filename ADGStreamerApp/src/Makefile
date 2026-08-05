TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += ADGStreamer

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install ADGStreamer.dbd into <top>/dbd
DBD += ADGStreamer.dbd

# specify all source files to be compiled and added to the library
#ADGStreamer_SRCS += xxx

ADGStreamer_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

