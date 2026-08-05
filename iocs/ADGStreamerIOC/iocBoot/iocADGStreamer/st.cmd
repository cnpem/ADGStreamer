#!../../bin/linux-x86_64/ADGStreamer

< envPaths

# Prefix
epicsEnvSet("P", "SWC:A:ADGST01:")
# The port name for the detector
epicsEnvSet("PORT", "ADGStreamer")
# The queue size for all plugins
epicsEnvSet("QSIZE","400")
# The search path for database files
epicsEnvSet("EPICS_DB_INCLUDE_PATH", "${ADCORE}/db:${ADGSTREAMER}/db")

## Register all support components
dbLoadDatabase("../../dbd/ADGStreamer.dbd")
ADGStreamer_registerRecordDeviceDriver(pdbbase)

# ADGStreamerDrive
# Port name
# Max buffers
# Max memory
# Thread priority
# Thread stack size
ADGStreamerDrive("$(PORT)", 0, 0, 0, 0)
dbLoadRecords("ADGStreamer.template", "P=${P},R=cam1:,PORT=${PORT},ADDR=0,TIMEOUT=1")

# ADGStreamerConfigureRTSP
# Port name
# Username
# Password
# Host
# Port
# Path
# Protocol: 0 = TCP, 1 = UDP
# RTSP latency (ms)
# Drop frames on latency: 0 = false, 1 = true
# Codec: 0 = H.264, 1 = H.265, 2 = MJPEG
# Queue size (buffers)
# Queue leaky mode: 0 = none, 1 = upstream, 2 = downstream
ADGStreamerConfigureRTSP("$(PORT)", "admin", "r00tr00t", "10.20.21.40", 554, "/cam/realmonitor?channel=1&subtype=0", 1, 0, 1, 0, 1, 2)

# Create PV Access conversion plugin
NDPvaConfigure("PVA1", ${QSIZE}, 0, "${PORT}", 0, ${P}Pva1:Image, 0, 0, 0)
dbLoadRecords("NDPva.template", "P=${P}, R=Pva1:, PORT=PVA1, ADDR=0, TIMEOUT=1, NDARRAY_PORT=${PORT}")

# Create an Image plugin
NDStdArraysConfigure("Image1", ${QSIZE}, 0, "${PORT}", 0, 0)
dbLoadRecords("NDStdArrays.template", "P=${P},R=image1:,PORT=Image1,NDARRAY_PORT=${PORT},ADDR=0,TIMEOUT=1,TYPE=Int8,FTVL=UCHAR,NELEMENTS=10000000")

# Create an HDF5 file saving plugin
#NDFileHDF5Configure("FileHDF1", ${QSIZE}, 0, "${PORT}", 0)
#dbLoadRecords("NDFileHDF5.template","P=${P},R=HDF1:,PORT=FileHDF1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=${PORT}")

# Turn on asyn trace
#asynSetTraceMask("${PORT}",0,0x21)
#asynSetTraceIOMask("${PORT}",0,1)

iocInit()

#Enable Array Callbacks, set Attributes file
dbpf("${P}cam1:ArrayCallbacks","Enable")

#Enable Array Callbacks image plugin
dbpf("${P}image1:EnableCallbacks","Enable")

#Enable Array Callbacks Pva plugin
dbpf("${P}Pva1:EnableCallbacks","Enable")

#Enable Array Callbacks HDF1 plugin
#dbpf("${P}HDF1:EnableCallbacks","Enable")

# Start acquisition
#dbpf("${P}cam1:Acquire","1")
