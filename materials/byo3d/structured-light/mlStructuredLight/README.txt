
Structured Light for 3D Scanning
http://mesh.brown.edu/byo3d/index.html

Douglas Lanman
dlanman@gmail.com        
Brown University

Version 1.0 (1 July 2009)

--------
Overview
--------

Structured lighting is a conceptually-simple, yet practical, approach for non-contact optical 3D scanning. This software uses Gray codes, composed of binary image sequences, to allow a 3D surface model to be recovered using a single projector-camera pair. Each bit-plane of the Gray code sequence is projected sequentially; afterwards, the sequence of camera images is decoded to resolve the correspondence between each camera pixel and each projector row and/or column. The intrinsic parameters (i.e., focal length, skew, and scale factors) and extrinsic parameters (i.e., rotation and translation) of the camera and projector are calibrated by observing a known printed chessboard pattern, as well as a known projected chessboard pattern as it appears distorted on the physical chessboard surface. Given the projector-camera calibration and correspondences, a 3D model is recovered by optical triangulation.

This software is implemented using Matlab and has the following main functions.

   - Manual intrinsic camera calibration
   - Manual intrinsic projector calibration
   - Manual extrinsic projector-camera calibration
   - 3D scanning with Gray codes using "ray-plane" triangulation 
   - 3D scanning with Gray codes using "ray-ray" triangulation

This software was originally presented as part of the following course.

   Build Your Own 3D Scanner: 3D Photography for Beginners
   Douglas Lanman and Gabriel Taubin
   ACM SIGGRAPH 2009 Course Notes

The latest release and additional details are available on the course website.

   http://mesh.brown.edu/byo3d/index.html

-------------------
System Requirements
-------------------

This software has been successfully run on the following systems.

   - Windows
     > XP Service Pack 3
         MATLAB Version 7.2.0.232 (R2006a)
         Image Acquisition Toolbox for MATLAB
	   Camera Calibration Toolbox for MATLAB
         Psychophysics Toolbox Version 3 (PTB-3)
         
In addition to the operating system, any camera can be used that is supported by the Image Acquisition Toolbox. Currently, the following cameras have been tested.

   - Logitech QuickCam Pro 9000

Finally, any digital projector that is recognized by the operating system should 
work. Currently, the following projectors have been tested.

   - InFocus LP70 and LP70+   
   - Mitsubishi PK10 Pocket Projector

--------------------
Running the Software
--------------------

These directions describe how to configure and use the provided software in Windows XP. This document will be updated in the future to describe similar procedures for other operating systems.

Begin by installing all required toolboxes, including:

1) Image Acquisition Toolbox for MATLAB (http://www.mathworks.com/products/imaq/)
2) Camera Calibration Toolbox for MATLAB (http://www.vision.caltech.edu/bouguetj/calib_doc/)
3) Psychophysics Toolbox Version 3 (http://psychtoolbox.org/wikka.php?wakka=HomePage)

The Image Acquisition Toolbox will allow image capture from almost any camera supported by the operating system (for details see the toolbox documentation). Similarly, the Psychophysics Toolbox provides OpenGL wrappers to control almost any projector supported by the operating system; note that your video card must support OpenGL to use this toolbox. Finally, the Camera Calibration Toolbox provides the calibration routines necessary for modeling intrinsic, extrinsic, and distortion parameters for cameras and projectors.

Detailed directions on using this toolbox are provided in the official course notes. Please consult Chapter 3 for instructions on using the camera and projector calibration functions. See Chapter 5 for instructions on using the scanning function "slScan.m".

-------------------
Release Information
-------------------

This software was originally developed by Douglas Lanman at Brown University in June of 2009. Brief revision notes follow.

   - Version 1.0 (1 July 2009)
     > First release supporting projector-camera calibration and reconstruction.

----------------
Acknowledgements
----------------

Gabriel Taubin originally conceived and co-developed the "Build your Own 3D  Scanner" course, as presented at SIGGRAPH 2009. In addition, Matt Hirsch, Ankit Mohan, and Ramesh Raskar provided useful comments that influenced the design of the software provided in that course, including this release.

-------
License
-------

Copyright (c) 2009 Douglas Lanman

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.