#ifndef RTSP_PIPELINE_BUILDER_H
#define RTSP_PIPELINE_BUILDER_H

#include <string>

#include "PipelineBuilder.h"
#include "PipelineTypes.h"

class PipelineBuilderRTSP : public PipelineBuilder {
public:
    PipelineBuilderRTSP();

    PipelineBuilderRTSP &setAuthentication(
        const std::string &username, const std::string &password);
    PipelineBuilderRTSP &setHost(
        const std::string &host, int port, const std::string &path);
    PipelineBuilderRTSP &setProtocol(ProtocolType protocol);
    PipelineBuilderRTSP &setLatency(int latency);
    PipelineBuilderRTSP &setDropOnLatency(bool enable);
    PipelineBuilderRTSP &setCodec(CodecType codec);

    std::string build() const override;

private:
    std::string username_;
    std::string password_;
    std::string host_;
    std::string path_;
    int port_;

    ProtocolType protocol_;
    CodecType codec_;
    int latency_;
    bool dropOnLatency_;
};

#endif