#include "ADGStreamer.h"

static const char *driverName = "ADGStreamer";

ADGStreamer::ADGStreamer(const char *portName, int maxBuffers, size_t maxMemory,
    int priority, int stackSize)
    : ADDriver(portName, 1, 1, maxBuffers, maxMemory, 0, 0, ASYN_CANBLOCK, 1,
          priority, stackSize)
{
    initializeGStreamer();

    epicsThreadCreate("ADGSTAcquire",
                            epicsThreadPriorityMedium,
                            epicsThreadGetStackSize(epicsThreadStackMedium),
                            (EPICSTHREADFUNC)acquisitionTaskC,
                            this);
}

ADGStreamer::~ADGStreamer() { stopPipeline(); }

asynStatus ADGStreamer::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;

    if (function == ADAcquire) {
        if (value) {
            startPipeline();
        } else {
            stopPipeline();
        }
    }

    return ADDriver::writeInt32(pasynUser, value);
}

void ADGStreamer::acquisitionTaskC(void *drvPvt)
{
    ADGStreamer *pPvt = static_cast<ADGStreamer *>(drvPvt);
    pPvt->acquisitionTask();
}

void ADGStreamer::acquisitionTask()
{
    int acquire;
    epicsTimeStamp startTime, endTime;
    double elapsedTime, delay;

    while (true)
    {
        epicsTimeGetCurrent(&startTime);

        lock();
        getIntegerParam(ADAcquire, &acquire);
        if (acquire)
        {
        }
        else
        {
        }
        unlock();

        epicsTimeGetCurrent(&endTime);
        elapsedTime = epicsTimeDiffInSeconds(&endTime, &startTime);
        delay = 0.1 - elapsedTime;
        epicsThreadSleep(delay);
    }
}

bool ADGStreamer::initializeGStreamer()
{
    gst_init(nullptr, nullptr);
    return true;
}

bool ADGStreamer::startPipeline()
{
    if (pipeline_) {
        return false;
    }

    if (!createPipeline()) {
        return false;
    }

    firstSample_ = true;

    GstStateChangeReturn ret;
    ret = gst_element_set_state(pipeline_, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        return false;
    }

    GstState state;
    gst_element_get_state(pipeline_, &state, nullptr, GST_CLOCK_TIME_NONE);

    if (state != GST_STATE_PLAYING) {
        return false;
    }

    return true;
}

bool ADGStreamer::stopPipeline()
{
    if (pipeline_) {
        GstStateChangeReturn ret;
        ret = gst_element_set_state(pipeline_, GST_STATE_NULL);

        if (ret == GST_STATE_CHANGE_FAILURE) {
            return false;
        }

        GstState state;
        gst_element_get_state(pipeline_, &state, nullptr, GST_CLOCK_TIME_NONE);

        if (state != GST_STATE_NULL) {
            return false;
        }
    }

    if (sink_) {
        gst_object_unref(sink_);
        sink_ = nullptr;
    }

    if (pipeline_) {
        gst_object_unref(pipeline_);
        pipeline_ = nullptr;
    }

    return true;
}

bool ADGStreamer::createPipeline()
{
    std::string pipeline
        = std::string("rtspsrc "
                      "location=rtsp://admin:r00tr00t@10.20.21.40:554/cam/"
                      "realmonitor?channel=1&subtype=0 protocols=udp latency=0 "
                      "drop-on-latency=true")
        + std::string(" ! rtph264depay ! h264parse config-interval=-1")
        + std::string(
            " ! queue max-size-buffers=1 leaky=downstream silent=true")
        + std::string(" ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! "
                      "appsink name=appsink sync=false");

    GError *error = nullptr;
    pipeline_ = gst_parse_launch(pipeline.c_str(), &error);

    if (!pipeline_) {
        if (error) { }
        return false;
    }

    if (error) {
        return false;
    }

    sink_ = gst_bin_get_by_name(GST_BIN(pipeline_), "appsink");

    if (!sink_) {
        return false;
    }

    gst_app_sink_set_emit_signals(GST_APP_SINK(sink_), true);
    gst_app_sink_set_drop(GST_APP_SINK(sink_), true);
    gst_app_sink_set_max_buffers(GST_APP_SINK(sink_), 1);

    g_signal_connect(
        sink_, "new-sample", G_CALLBACK(onNewSampleCallback), this);

    return true;
}

GstFlowReturn ADGStreamer::onNewSampleCallback(
    GstAppSink *sink, gpointer userData)
{
    ADGStreamer *pPvt = static_cast<ADGStreamer *>(userData);
    return pPvt->onNewSample();
}

GstFlowReturn ADGStreamer::onNewSample()
{
    GstSample *sample = nullptr;

    lock();

    if (!sink_) {
        return GST_FLOW_ERROR;
    }

    sample = gst_app_sink_pull_sample(GST_APP_SINK(sink_));

    if (!sample) {
        return GST_FLOW_ERROR;
    }

    processSample(sample);
    gst_sample_unref(sample);

    unlock();

    return GST_FLOW_OK;
}

bool ADGStreamer::processSample(GstSample *sample)
{
    GstBuffer *buffer = nullptr;
    GstMapInfo map;
    int imageCounter_;
    int numImagesCounter_;
    int arrayCallbacks_;
    NDArray *pImage_;
    epicsTimeStamp startTime;

    epicsTimeGetCurrent(&startTime);

    buffer = gst_sample_get_buffer(sample);

    if (!buffer) {
        return false;
    }

    GstCaps *caps = gst_sample_get_caps(sample);

    if (caps && firstSample_) {
        if (!updateSample(caps)) {
            return false;
        }
        firstSample_ = false;
    }

    if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        return false;
    }

    pImage_ = this->pNDArrayPool->alloc(ndims_, dims_, dataType_, 0, NULL);

    if (!pImage_) {
        gst_buffer_unmap(buffer, &map);
        return false;
    }

    memcpy(pImage_->pData, map.data, pImage_->dataSize);
    gst_buffer_unmap(buffer, &map);

    getIntegerParam(NDArrayCounter, &imageCounter_);
    getIntegerParam(ADNumImagesCounter, &numImagesCounter_);
    getIntegerParam(NDArrayCallbacks, &arrayCallbacks_);
    imageCounter_++;
    numImagesCounter_++;
    setIntegerParam(NDArrayCounter, imageCounter_);
    setIntegerParam(ADNumImagesCounter, numImagesCounter_);
    pImage_->uniqueId = imageCounter_;
    pImage_->timeStamp = startTime.secPastEpoch + startTime.nsec / 1.e9;
    updateTimeStamp(&pImage_->epicsTS);

    this->getAttributes(pImage_->pAttributeList);
    if (arrayCallbacks_) {
        doCallbacksGenericPointer(pImage_, NDArrayData, 0);
    }
    pImage_->release();

    return true;
}

bool ADGStreamer::updateSample(GstCaps *caps)
{
    GstStructure *structure;
    structure = gst_caps_get_structure(caps, 0);

    if (!structure) {
        return false;
    }

    gint width = 0;
    gint height = 0;
    gst_structure_get_int(structure, "width", &width);
    gst_structure_get_int(structure, "height", &height);

    epicsInt32 imageWidth, imageHeight;
    if (width > 0 && height > 0) {
        imageWidth = width;
        imageHeight = height;
        setIntegerParam(ADSizeX, imageWidth);
        setIntegerParam(ADSizeY, imageHeight);
        setIntegerParam(NDArraySizeX, imageWidth);
        setIntegerParam(NDArraySizeY, imageHeight);
    } else {
        return false;
    }

    const gchar *format;
    format = gst_structure_get_string(structure, "format");
    if (format) {
        if (strcmp(format, "GRAY8") == 0) {
            ndims_ = 2;
            dims_[0] = imageWidth;
            dims_[1] = imageHeight;
            dims_[2] = 2;
            dataType_ = NDUInt8;
            setIntegerParam(NDDataType, dataType_);
            setIntegerParam(NDColorMode, NDColorModeMono);
            setIntegerParam(NDArraySize, imageWidth * imageHeight);
        } else if (strcmp(format, "RGB") == 0) {
            ndims_ = 3;
            dims_[0] = 3;
            dims_[1] = imageWidth;
            dims_[2] = imageHeight;
            dataType_ = NDUInt8;
            setIntegerParam(NDDataType, dataType_);
            setIntegerParam(NDColorMode, NDColorModeRGB1);
            setIntegerParam(NDArraySize, imageWidth * imageHeight * 3);
        }
    } else {
        return false;
    }

    setIntegerParam(NDArrayCounter, 0);
    setIntegerParam(ADNumImagesCounter, 0);

    callParamCallbacks();

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
