#ifndef PIPELINE_TYPES_H
#define PIPELINE_TYPES_H

enum ProtocolType { PROTOCOL_TCP = 0, PROTOCOL_UDP = 1 };

enum CodecType { CodecH264 = 0, CodecH265 = 1, CodecMJPEG = 2 };

enum QueueLeaky { LEAKY_NONE = 0, LEAKY_UPSTREAM = 1, LEAKY_DOWNSTREAM = 2 };

enum ColorModeType {
    COLOR_GRAY8 = 0,
    COLOR_RGB = 1,
};

#endif
