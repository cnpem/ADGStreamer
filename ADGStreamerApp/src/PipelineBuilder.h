#ifndef PIPELINE_BUILDER_H
#define PIPELINE_BUILDER_H

#include "PipelineTypes.h"
#include <string>

class PipelineBuilder {
public:
    PipelineBuilder();
    virtual ~PipelineBuilder() = default;
    virtual std::string build() const;

    PipelineBuilder &setQueue(int queueSize, QueueLeaky leaky);
    PipelineBuilder &setVideoConvert(ColorModeType colorMode);

private:
    int queueSize_;
    QueueLeaky leaky_;
    ColorModeType colorMode_;
};

#endif