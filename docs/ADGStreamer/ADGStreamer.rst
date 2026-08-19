ADGStreamer
===========

Overview
--------

ADGStreamer is an AreaDetector driver that uses the GStreamer multimedia
framework to acquire images from IP cameras and other streaming sources.

Unlike most existing AreaDetector drivers, ADGStreamer separates the
communication protocol from the driver implementation through the
PipelineBuilder architecture. This allows different streaming protocols
to share the same acquisition logic while implementing only the
protocol-specific pipeline stages.

The driver automatically detects image width, height, color mode and
data type from the received stream and publishes the images as NDArrays.

Features
--------

The current implementation provides the following features:

- Native AreaDetector driver.
- GStreamer-based acquisition.
- Automatic image size detection.
- Automatic color mode detection.
- Automatic NDDataType detection.
- PipelineBuilder architecture.
- RTSP support.

Future versions are expected to support additional communication
protocols including HTTP, RTMP and SRT.

Architecture
------------

The driver is divided into two independent layers.

The first layer is responsible for the communication protocol.

The second layer contains the common image processing stages shared by
all protocols.

::

        +-----------------------------+
        |         ADDriver            |
        +-------------+---------------+
                      |
                      v
              +---------------+
              | ADGStreamer   |
              +-------+-------+
                      |
                      v
             +------------------+
             | PipelineBuilder  |
             +--------+---------+
                      |
          +-----------+-----------+
          |                       |
          v                       v
 PipelineBuilderRTSP      PipelineBuilderHTTP (not yet supported)
          |                       |
          +-----------+-----------+
                      |
                      v
               GStreamer Pipeline
                      |
                      v
                   AppSink
                      |
                      v
                  NDArrayPool

Supported Protocols
-------------------

================== ===========
Protocol           Status
================== ===========
RTSP               Supported
HTTP               Planned
RTMP               Planned
SRT                Planned
================== ===========

Requirements
------------

The driver requires GStreamer 1.x together with the standard plugin
packages.

Typical Debian installation:

::

    sudo apt install \
        libgstreamer1.0-dev \
        libgstreamer-plugins-base1.0-dev \
        gstreamer1.0-tools \
        gstreamer1.0-plugins-base \
        gstreamer1.0-plugins-good \
        gstreamer1.0-plugins-bad \
        gstreamer1.0-plugins-ugly \
        gstreamer1.0-libav

Driver Parameters
-----------------

The driver uses the standard AreaDetector parameters.

Additional parameters include:

======================= ===============================================
Parameter               Description
======================= ===============================================
PipelineBuilder_RBV     Generated GStreamer pipeline
======================= ===============================================

IOC Configuration
-----------------

The driver is configured using IOC shell commands.

Example:

::

    ADGStreamerDrive(
        Port name,
        Max buffers,
        Max memory,
        Thread priority,
        Thread stack size,
    )

    ADGStreamerConfigureRTSP(
        Port name,
        Username,
        Password,
        Host,
        Port,
        Path,
        Protocol,
        latency,
        Drop frames on latency,
        Codec,
        Queue size,
        Queue leaky mode)
