#include "PipelineBuilder.h"

#include <sstream>

PipelineBuilder::PipelineBuilder()
    : leaky_(LEAKY_NONE)
    , queueSize_(1)
    , colorMode_(COLOR_RGB)
{
}

PipelineBuilder &PipelineBuilder::setQueue(int size, QueueLeaky leaky)
{
    queueSize_ = size;
    leaky_ = leaky;
    return *this;
}

PipelineBuilder &PipelineBuilder::setVideoConvert(ColorModeType colorMode)
{
    colorMode_ = colorMode;
    return *this;
}

std::string PipelineBuilder::build() const
{
    std::stringstream pipeline;

    /*--------------------------------------------------------------
     * Queue
     *-------------------------------------------------------------*/

    pipeline << " ! queue";

    pipeline << " max-size-buffers=" << queueSize_;

    pipeline << " leaky=";

    switch (leaky_) {
    case LEAKY_NONE:
        pipeline << "no";
        break;

    case LEAKY_UPSTREAM:
        pipeline << "upstream";
        break;

    case LEAKY_DOWNSTREAM:
        pipeline << "downstream";
        break;

    default:
        pipeline << "no";
        break;
    }

    /*--------------------------------------------------------------
     * Decoder
     *-------------------------------------------------------------*/

    pipeline << " ! decodebin";

    /*--------------------------------------------------------------
     * Convert
     *-------------------------------------------------------------*/

    pipeline << " ! videoconvert";

    switch (colorMode_) {
    case COLOR_GRAY8:
        pipeline << " ! video/x-raw,format=GRAY8";
        break;

    case COLOR_RGB:
        pipeline << " ! video/x-raw,format=RGB";
        break;

    default:
        pipeline << " ! video/x-raw,format=RGB";
        break;
    }

    /*--------------------------------------------------------------
     * Sink
     *-------------------------------------------------------------*/

    pipeline << " ! appsink";
    pipeline << " name=appsink";
    pipeline << " sync=false";

    return pipeline.str();
}
