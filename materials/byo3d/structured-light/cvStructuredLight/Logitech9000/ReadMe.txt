Raw mode enabling/disabling tool for Logitech QuickCam devices
==============================================================

Copyright 2007-2008 (c) Logitech. All Rights Reserved.

This code and information is provided "as is" without warranty of
any kind, either expressed or implied, including but not limited to
the implied warranties of merchantability and/or fitness for a
particular purpose.


Introduction
============

This C++ sample tool demonstrates how to access extension unit controls through DirectShow
in order to control raw mode ("Bayer mode") of Logitech QuickCam UVC camera devices.

There is a how-to article with more details to go with this sample. You can find it here:
http://www.quickcamteam.net/documentation/how-to/how-to-enable-raw-streaming-on-logitech-webcams


Notes
=====

Please read the mentioned how-to article to get familiar with the features and limitations
of this tool. In particular, keep in mind the following important points:

- Settings changed with this tool does not have any effect while the camera is streaming video.
  The video stream must be restarted for the settings to take effect.

- Raw mode only works in certain resolutions. For 1.3 MP cameras, those will usually be the
  camera's "native" resolutions, i.e. 960x720 or 1280x960. For 2 MP cameras more resolutions
  may be supported. If color processing is disabled (i.e. "Bayer mode" is enabled) and
  streaming is started in a not supported resolution, the camera sends no data and the
  application may appear to hang. If this happens, just choose another resolution.

- If Bayer images are captured in any resolution lower than the maximum resolution supported
  by the sensor, clipping occurs. For example, if the sensor supports 1280x960 and a 960x720
  image is recorded, the frame will appear clipped at the right and lower borders.

If you have questions or problems with this sample, you can post them in the QuickCam Team
Windows Webcam Development forum at: http://forums.quickcamteam.net/
We do not guarantee support for this sample but we will try to respond to your questions.


Prerequisites
=============

For compilation:
- Visual Studio 2005
- A recent version of the Windows SDK (tested with 6.0)

The tool was tested on a Windows XP with SP2 and Windows Vista with SP1.


Release history
===============

1.0: First release to the public
