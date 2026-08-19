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
ADGStreamer_SRCS += ADGStreamer.cpp
ADGStreamer_SRCS += PipelineBuilder.cpp
ADGStreamer_SRCS += PipelineBuilderRTSP.cpp

USR_CXXFLAGS += $(shell pkg-config --cflags gstreamer-1.0)
ADGStreamer_SYS_LIBS += gstapp-1.0
ADGStreamer_SYS_LIBS += gstbase-1.0
ADGStreamer_SYS_LIBS += gstreamer-1.0
ADGStreamer_SYS_LIBS += gobject-2.0
ADGStreamer_SYS_LIBS += glib-2.0

ADGStreamer_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(ADCORE)/ADApp/commonLibraryMakefile

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

