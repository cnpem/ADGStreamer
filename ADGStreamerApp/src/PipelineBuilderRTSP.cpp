#include "PipelineBuilderRTSP.h"

#include <sstream>

PipelineBuilderRTSP::PipelineBuilderRTSP()
    : PipelineBuilder()
    , port_(554)
    , protocol_(PROTOCOL_TCP)
    , codec_(CodecH264)
    , latency_(0)
    , dropOnLatency_(false)
{
}

PipelineBuilderRTSP &PipelineBuilderRTSP::setAuthentication(
    const std::string &username, const std::string &password)
{
    username_ = username;
    password_ = password;
    return *this;
}

PipelineBuilderRTSP &PipelineBuilderRTSP::setHost(
    const std::string &host, int port, const std::string &path)
{
    host_ = host;
    port_ = port;
    path_ = path;
    return *this;
}

PipelineBuilderRTSP &PipelineBuilderRTSP::setProtocol(ProtocolType protocol)
{
    protocol_ = protocol;
    return *this;
}

PipelineBuilderRTSP &PipelineBuilderRTSP::setLatency(int latency)
{
    latency_ = latency;
    return *this;
}

PipelineBuilderRTSP &PipelineBuilderRTSP::setDropOnLatency(bool enable)
{
    dropOnLatency_ = enable;
    return *this;
}

PipelineBuilderRTSP &PipelineBuilderRTSP::setCodec(CodecType codec)
{
    codec_ = codec;
    return *this;
}

std::string PipelineBuilderRTSP::build() const
{
    std::stringstream pipeline;

    /*--------------------------------------------------------------
     * Source
     *-------------------------------------------------------------*/

    pipeline << "rtspsrc";

    pipeline << " location=rtsp://";

    if (!username_.empty()) {
        pipeline << username_;

        if (!password_.empty()) {
            pipeline << ":" << password_;
        }

        pipeline << "@";
    }

    pipeline << host_;

    if (port_ > 0) {
        pipeline << ":" << port_;
    }

    pipeline << path_;

    pipeline << " protocols=" << (protocol_ == PROTOCOL_TCP ? "tcp" : "udp");

    pipeline << " latency=" << latency_;

    pipeline << " drop-on-latency=" << (dropOnLatency_ ? "true" : "false");

    /*--------------------------------------------------------------
     * Depayloader
     *-------------------------------------------------------------*/

    switch (codec_) {
    case CodecH264:
        pipeline << " ! rtph264depay";
        break;

    case CodecH265:
        pipeline << " ! rtph265depay";
        break;

    case CodecMJPEG:
        pipeline << " ! rtpjpegdepay";
        break;

    default:
        pipeline << " ! rtph264depay";
        break;
    }

    /*--------------------------------------------------------------
     * Parser
     *-------------------------------------------------------------*/

    switch (codec_) {
    case CodecH264:
        pipeline << " ! h264parse config-interval=-1";
        break;

    case CodecH265:
        pipeline << " ! h265parse";
        break;

    case CodecMJPEG:
        break;

    default:
        pipeline << " ! h264parse config-interval=-1";
        break;
    }

    pipeline << PipelineBuilder::build();

    return pipeline.str();
}
