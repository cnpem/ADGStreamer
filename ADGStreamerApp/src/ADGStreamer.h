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

    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);

protected:
private:
    bool initializeGStreamer();

    bool startPipeline();
    bool stopPipeline();
    bool createPipeline();

    static GstFlowReturn onNewSampleCallback(
        GstAppSink *sink, gpointer userData);
    GstFlowReturn onNewSample();

    GstElement *pipeline_;
    GstElement *sink_;
};

#endif
