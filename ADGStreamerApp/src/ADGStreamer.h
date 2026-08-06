#ifndef ADGSTREAMER_H
#define ADGSTREAMER_H

#include <epicsExport.h>
#include <iocsh.h>

#include <ADDriver.h>

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

#include "PipelineBuilder.h"
#include "PipelineBuilderRTSP.h"
#include "PipelineTypes.h"

class ADGStreamer : public ADDriver {

public:
    ADGStreamer(const char *portName, int maxBuffers = 0, size_t maxMemory = 0,
        int priority = 0, int stackSize = 0);
    virtual ~ADGStreamer();

    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);

    bool pipelineBuilder(PipelineBuilder *builder);

protected:
private:
    static void acquisitionTaskC(void *drvPvt);
    void acquisitionTask();

    bool initializeGStreamer();

    bool startPipeline();
    bool stopPipeline();
    bool createPipeline();

    static GstFlowReturn onNewSampleCallback(
        GstAppSink *sink, gpointer userData);
    GstFlowReturn onNewSample();

    bool processSample(GstSample *sample);
    bool updateSample(GstCaps *caps);

    bool processBusMessage();

    GstElement *pipeline_;
    GstElement *sink_;
    GstBus *bus_;

    bool firstSample_;

    int ndims_;
    size_t dims_[3];
    NDDataType_t dataType_;

    PipelineBuilder *pipelineBuilder_;
};

#endif
