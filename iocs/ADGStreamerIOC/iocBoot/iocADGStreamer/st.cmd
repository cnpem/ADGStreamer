#!../../bin/linux-x86_64/ADGStreamer

#- You may have to change ADGStreamer to something else
#- everywhere it appears in this file

#< envPaths

## Register all support components
dbLoadDatabase "../../dbd/ADGStreamer.dbd"
ADGStreamer_registerRecordDeviceDriver(pdbbase) 

## Load record instances
#dbLoadRecords("../../db/ADGStreamer.db","user=root")

iocInit()

## Start any sequence programs
#seq sncADGStreamer,"user=root"
