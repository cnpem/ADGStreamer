#ifndef ADGSTREAMER_H
#define ADGSTREAMER_H

#include <epicsExport.h>
#include <iocsh.h>

#include <ADDriver.h>

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

class ADGStreamer : public ADDriver {

public:
    ADGStreamer(const char *portName, int maxBuffers = 0, size_t maxMemory = 0,
        int priority = 0, int stackSize = 0);
    virtual ~ADGStreamer();

protected:
private:
    bool initializeGStreamer();
};

#endif
