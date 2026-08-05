#include "ADGStreamer.h"

static const char *driverName = "ADGStreamer";

ADGStreamer::ADGStreamer(const char *portName, int maxBuffers, size_t maxMemory,
    int priority, int stackSize)
    : ADDriver(portName, 1, 1, maxBuffers, maxMemory, 0, 0, ASYN_CANBLOCK, 1,
          priority, stackSize)
{
    initializeGStreamer();
}

ADGStreamer::~ADGStreamer() { }

bool ADGStreamer::initializeGStreamer()
{
    gst_init(nullptr, nullptr);
    return true;
}

extern "C" int ADGStreamerDrive(const char *portName, int maxBuffers,
    size_t maxMemory, int priority, int stackSize)
{
    new ADGStreamer(portName, maxBuffers, maxMemory, priority, stackSize);
    return asynSuccess;
}

static const iocshArg ADGStreamerDriveArg0 = { "Port name", iocshArgString };
static const iocshArg ADGStreamerDriveArg1 = { "Max buffers", iocshArgInt };
static const iocshArg ADGStreamerDriveArg2 = { "Max memory", iocshArgInt };
static const iocshArg ADGStreamerDriveArg3 = { "Priority", iocshArgInt };
static const iocshArg ADGStreamerDriveArg4 = { "Stack size", iocshArgInt };

static const iocshArg *const ADGStreamerDriveArgs[]
    = { &ADGStreamerDriveArg0, &ADGStreamerDriveArg1, &ADGStreamerDriveArg2,
          &ADGStreamerDriveArg3, &ADGStreamerDriveArg4 };

static const iocshFuncDef ADGStreamerDriveFuncDef
    = { "ADGStreamerDrive", 5, ADGStreamerDriveArgs };

static void ADGStreamerDriveCallFunc(const iocshArgBuf *args)
{
    ADGStreamerDrive(
        args[0].sval, args[1].ival, args[2].ival, args[3].ival, args[4].ival);
}

static void ADGStreamerRegister()
{
    iocshRegister(&ADGStreamerDriveFuncDef, ADGStreamerDriveCallFunc);
}

epicsExportRegistrar(ADGStreamerRegister);
