ADGStreamer
=======

An [EPICS][epics] [areaDetector][] driver for the [GStreamer][profilers] to acquire images from network video streams and publish them as **NDArrays** through **EPICS**.

Currently, ADGStreamer supports **RTSP** communication only and provides configuration options for several pipeline parameters, including: protocol, latency (ms), drop frames on latency, codec, queue size (buffers) and queue leaky mode.

[profilers]: https://gstreamer.freedesktop.org/
[epics]: https://docs.epics-controls.org/en/latest/
[areaDetector]: https://github.com/areaDetector/areaDetector/blob/master/README.md

Additional information:
- [Documentation](docs/ADGStreamer/ADGStreamer.rst)
- [Release notes](RELEASE.md)