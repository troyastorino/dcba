
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

This software is implemented using OpenCV and has the following main functions.

   - Automatic intrinsic camera calibration
   - Automatic intrinsic projector calibration
   - Automatic extrinsic projector-camera calibration
   - 3D scanning with Gray codes using "ray-plane" triangulation 
   - 3D scanning with Gray codes using "ray-ray" triangulation
   - Background modeling for automatic region of interest selection

This software was originally presented as part of the following course.

   Build Your Own 3D Scanner: 3D Photography for Beginners
   Douglas Lanman and Gabriel Taubin
   ACM SIGGRAPH 2009 Course Notes

The latest release and additional details are available on the course website.

   http://mesh.brown.edu/byo3d/index.html

-------------------
System Requirements
-------------------

This software has been successfully compiled and run on the following systems.

   - Windows
     > XP Service Pack 3
	 Visual Studio C++ 2005
	 OpenCV 1.1pre1

In addition to the operating system, any camera can be used that is supported by OpenCV. Currently, the following cameras have been tested.

   - Logitech QuickCam Pro 9000

Finally, any digital projector that is recognized by the operating system should 
work. For example, in Windows, the projector is assumed to be a secondary display 
and must be located to the left of the primary display (see following section). 
Currently, the following projectors have been tested.

   - InFocus LP70 and LP70+   
   - Mitsubishi PK10 Pocket Projector

--------------------
Running the Software
--------------------

These directions describe how to configure and use the provided software in Windows XP. This document will be updated in the future to describe similar procedures for other operating systems.

---------------------------------
> Step 1: Configuring your System

Begin by installing OpenCV 1.1pre1 from http://sourceforge.net/projects/opencvlibrary/. 

Before using the software, the projector and camera must be configured. A list of cameras supported by OpenCV is maintained at http://opencv.willowgarage.com/wiki/. The projector should be configured as a secondary display (i.e., it cannot be the primary desktop display). In addition, due to assumptions made in the provided software, the projector screen must be located immediately to the left of the primary display; furthermore, the "top" of each display should be aligned. Graphically, the "Display Properties" dialogue should show the following configuration:

   +--------------------++--------------------+
   |                    ||                    |
   |                    ||                    |
   |     Display #2     ||     Display #1     | 
   |     Projector      ||    User Desktop    | 
   |                    ||                    |
   |                    |+--------------------+
   |                    |
   +--------------------+

---------------------------------
> Step 2: Calibrating your Camera

Begin by printing a calibration chessboard pattern. For example, the default OpenCV chessboard pattern is provided in the "pattern" subdirectory. Note that this chessboard consists of 9x7 squares, each 30 mm x 30 mm. For projector calibration, the default pattern must be printed at reduced contrast (i.e., the squares should be gray, rather than black). The amount of required chessboard contrast may vary based on application. Image manipulation programs, such as Photoshop, the GIMP, or Irfanview can be used to further adjust the contrast of the provided chessboard pattern. The printed chessboard should be affixed to a rigid planar surface (e.g., a textbook or foamcore section).

Begin by opening "config.xml" in a text editor. Note that this file can be used to set various options for the structured lighting program. Note that the output directory is given by the following entries:

<output>
  <output_directory>"./output"</output_directory>
  <object_name>"demo"</object_name>
  <save_intermediate_results>0</save_intermediate_results></output>

The output_directory field defines where the results will be saved. The object_name field defines the subdirectory where results will be saved. The save_intermediate_results field enables/disables saving of intermediate data, such as images, depth maps, and related items.

The camera and projector options are given by the following entries in "config.xml":

<camera>
  <width>960</width>
  <height>720</height>
  <Logitech_Quickcam_9000_raw_mode>1</Logitech_Quickcam_9000_raw_mode></camera>
<projector>
  <width>1024</width>
  <height>768</height>
  <invert_projector>0</invert_projector></projector>

Note that the resolution of the projector and camera must correspond to modes supported by the physical devices. Only enable the Logitech_Quickcam_9000_raw_mode field if you are using a Logitech QuickCam 9000 that supports raw-mode capture, otherwise set this field to 0.

The physical properties of the printed chessboard must be defined by the following entries in "config.xml":

<camera_chessboard>
  <interior_horizontal_corners>8</interior_horizontal_corners>
  <interior_vertical_corners>6</interior_vertical_corners>
  <square_width_mm>30.</square_width_mm>
  <square_height_mm>30.</square_height_mm></camera_chessboard>

Update these fields so that they correspond with the printed chessboard you will use for calibration.

At this point you should be ready to run camera calibration. Begin by running "cvStructuredLight.exe" from the command prompt. If no errors occur, the following output should be reproduced:

[Structured Lighting for 3D Scanning]
Reading configuration file "./config.xml"...
Initializing camera and projector...
Enabling Bayer mode for Logitech QuickCam 9000...
Creating output directory (overwrites existing object data)...
Camera has not been intrinsically calibrated!
Projector has not been intrinsically calibrated!
Projector-camera system has not been extrinsically calibrated!

Press the following keys for the corresponding functions.
'S': Run scanner
'B': Estimate background
'R': Reset background
'C': Calibrate camera
'P': Calibrate projector
'A': Calibrate camera and projector simultaneously
'E': Calibrate projector-camera alignment
'ESC': Exit application

Press 'C' to run camera calibration. The following output should be reproduced:

> Calibrating camera...
Creating camera calibration directory (overwrites existing data)...
Enter the maximum number of calibration images, then press return.
+ Maximum number of images =

Enter the number of images you want to capture (typically about 10-20). A live preview window should appear. Move the chessboard into the scene. Detected corners will be shown with red circles. If the chessboard is found, then a colored set of lines will be displayed. Afterwards, you must press 'N' (while the camera preview window is active) to capture a frame. The camera gain slider can be adjusted if the chessboard corners are not being detected. Note that the camera driver should be used before calibration to disable auto-exposure, white balance, auto-focus, or similar adaptive settings.

After collecting a calibration sequence, the calibration routine will return the intrinsic parameters.

+ Captured frame 1 of 15.
+ Captured frame 2 of 15.
+ Captured frame 3 of 15.
+ Captured frame 4 of 15.
+ Captured frame 5 of 15.
+ Captured frame 6 of 15.
+ Captured frame 7 of 15.
+ Captured frame 8 of 15.
+ Captured frame 9 of 15.
+ Captured frame 10 of 15.
+ Captured frame 11 of 15.
+ Captured frame 12 of 15.
+ Captured frame 13 of 15.
+ Captured frame 14 of 15.
+ Captured frame 15 of 15.
Calibrating camera...
Saving calibration images and parameters...
Camera calibration was successful.
Camera calibration:
+ Intrinsic parameters =
   330.415   0.000 495.763
     0.000 320.929 304.657
     0.000   0.000   1.000
+ Distortion coefficients =
    -0.039   0.169  -0.024   0.138   0.000

The camera calibration results are saved to the "calib/cam" subdirectory of the user-defined output directory. Note that the intrinsic parameters can be used with the Camera Calibration Toolbox for Matlab, which has a similar imaging model.

------------------------------------
> Step 3: Calibrating your Projector

Once again, begin by editing "config.xml" in a text editor. Since projector calibration involves projecting a known chessboard, we must first define the size (in pixels) and number of squares of this chessboard using the following entries.

<projector_chessboard>
  <interior_horizontal_corners>8</interior_horizontal_corners>
  <interior_vertical_corners>6</interior_vertical_corners>
  <square_width_pixels>75</square_width_pixels>
  <square_height_pixels>75</square_height_pixels></projector_chessboard>

Once you have defined a chessboard for projection, run "cvStructuredLight.exe". Press 'P' to run projector calibration. The following output should be reproduced, if camera calibration was completed first:

[Structured Lighting for 3D Scanning]
Reading configuration file "./config.xml"...
Initializing camera and projector...
Enabling Bayer mode for Logitech QuickCam 9000...
Creating output directory (overwrites existing object data)...
Loaded previous intrinsic camera calibration.
Projector has not been intrinsically calibrated!
Projector-camera system has not been extrinsically calibrated!

Press the following keys for the corresponding functions.
'S': Run scanner
'B': Estimate background
'R': Reset background
'C': Calibrate camera
'P': Calibrate projector
'A': Calibrate camera and projector simultaneously
'E': Calibrate projector-camera alignment
'ESC': Exit application

Press 'P' to run projector calibration. The following output should be reproduced:

> Calibrating projector...
Creating projector calibration directory (overwrites existing data)...
Enter the maximum number of calibration images, then press return.
+ Maximum number of images =

At this point, enter the number of projector calibration images (typically 10-20). Place the calibration plane, with the same printed chessboard, into the scene. It is best to lean the board against something so that it doesn't move during the calibration procedure. If the printed chessboard is detected (using an identical procedure as camera calibration), then the projector will also project a "virtual" chessboard. Note that the brightness of the projector can be adjusted using the projector gain slider. Adjust the slider until the projector chessboard is clearly visible against the printed chessboard surface. As before, red circles will show detected corners of the projected chessboard. If the projected chessboard is found, then colored lines will be displayed. At this point, you can press 'N' (when the camera preview window is active) to capture a projector calibration frame. In general, you may need to adjust the camera gain (in the camera driver) and the projector brightness. Keep in mind that the calibration board should not move after you press 'N', until the frame has been saved. 

After collecting a projector calibration sequence, the following output should be reproduced:

+ Captured frame 1 of 15.
+ Captured frame 2 of 15.
+ Captured frame 3 of 15.
+ Captured frame 4 of 15.
+ Captured frame 5 of 15.
+ Captured frame 6 of 15.
+ Captured frame 7 of 15.
+ Captured frame 8 of 15.
+ Captured frame 9 of 15.
+ Captured frame 10 of 15.
+ Captured frame 11 of 15.
+ Captured frame 12 of 15.
+ Captured frame 13 of 15.
+ Captured frame 14 of 15.
+ Captured frame 15 of 15.
Calibrating projector...
Saving calibration images and parameters...
Projector calibration was successful.
Projector calibration:
+ Intrinsic parameters =
   1848.642   0.000 504.246
     0.000 1850.692 850.909
     0.000   0.000   1.000
+ Distortion coefficients =
    -0.207   0.370  -0.013   0.010   0.000

The projector calibration results are saved to the "calib/proj" subdirectory of the user-defined output directory. Note that the intrinsic parameters can be used with the Camera Calibration Toolbox for Matlab, which has a similar imaging model. Also note that the extrinsic calibration (i.e., transformation between camera and projector coordinate systems) will also be saved.

--------------------------------------------------------------
> Step 4 (Optional): Recalibrating your Projector's Extrinsics

If you have previously calibrated your projector and camera, but happen to move either, you will want to recalibrate the extrinsic parameters. If this happens, simply select 'E' from the main menu of "cvStructuredLight.exe". Note that a single "projector calibration image", as defined in Step 3, will be captured. Afterwards, the extrinsic parameters will be updated in the "calib/proj" subdirectory.

Also note that, rather than calibrating the camera and projector separately, the entire system can be calibrated using a single projector calibration sequence. This is due to the fact that such sequences contain both printed and projected chessboards---sufficient for estimating the intrinsic and extrinsic parameters of both the camera and projector. Select 'S' from the main menu of "cvStructuredLight.exe" to use this "fast" calibration mode.

----------------------------
> Step 5: Scanning an Object

Once again, begin by editing "config.xml" to edit the following reconstruction parameters:

<scanning_and_reconstruction>
  <mode>2</mode>
  <reconstruct_columns>1</reconstruct_columns>
  <reconstruct_rows>1</reconstruct_rows>
  <frame_delay_ms>300</frame_delay_ms>
  <minimum_contrast_threshold>50</minimum_contrast_threshold>
  <minimum_distance_mm>500.</minimum_distance_mm>
  <maximum_distance_mm>5000.</maximum_distance_mm>
  <maximum_distance_variation_mm>100.</maximum_distance_variation_mm>
  <minimum_background_distance_mm>20.</minimum_background_distance_mm>

The mode field selects between 1=ray-plane and 2=ray-ray triangulation (see the course notes for details). For fast scanning, select ray-plane triangulation and only reconstruct either the rows or the columns (e.g., to scan just columns set reconstruct_columns=1 and reconstruct_rows=0). If ray-ray triangulation is enabled these fields will be ignored.

The frame_delay_ms field controls the synchronization between the camera and projector. Since a hardware camera trigger was not assumed, the camera and projector run "open loop". As a result, we must wait some time after projecting a Gray code before capturing a frame from the camera. This delay, in milliseconds, is given by frame_delay_ms. Generally something around 100-400 ms should work for most systems.

The remaining fields are used to adjust the contrast threshold (i.e., minimum variation between a Gray code and its inverse) to consider a point "illuminated" by the projector. The minimum_distance_mm and maximum_distance_mm set the near and far clipping planes (and should be updated if you plan on scanning objects outside this range).

To scan an object, select 'S' from the main menu of "cvStructuredLight.exe". The following output should be reproduced:

[Structured Lighting for 3D Scanning]
Reading configuration file "./config.xml"...
Initializing camera and projector...
Enabling Bayer mode for Logitech QuickCam 9000...
Creating output directory (overwrites existing object data)...
Loaded previous intrinsic camera calibration.
Loaded previous intrinsic projector calibration.
Loaded previous extrinsic projector-camera calibration.

Press the following keys for the corresponding functions.
'S': Run scanner
'B': Estimate background
'R': Reset background
'C': Calibrate camera
'P': Calibrate projector
'A': Calibrate camera and projector simultaneously
'E': Calibrate projector-camera alignment
'ESC': Exit application

> Running scanner (view 1)...
Position object, then press any key (in 'camWindow') to scan.

Position your object so it is both illuminated by the projector and photographed by the camera. Afterwards, press any key in the camera preview window to start projecting the Gray code sequence(s).

Afterwards, the decoded rows and columns will be displayed. If these images do not seem reasonable, then you may need to edit the configuration file, adjust the brightness of the projector, or refine the camera driver settings. The depth map will be shaded according to the near and far clipping planes. The texture map and point clouds will be saved to the "object_name" subdirectory. Note that the BYO3D Java point cloud viewer can be used to view the output data (see the course notes). The following output should be reproduced after a successful scan:

Displaying the structured light sequence...
Decoding the structured light sequence...
Displaying the decoded columns; press any key (in 'camWindow') to continue.
Displaying the decoded rows; Press any key (in 'camWindow') to continue.
Reconstructing the point cloud and the depth map...
Displaying the depth map; press any key (in 'camWindow') to continue.
Saving the texture map...
Saving the point cloud...

--------------------------------------------------------------------
> Step 6 (Optional): Scanning an Object with Background Segmentation

Sometimes, you may have a solid background you want to remove from the output point clouds. This can be accomplished by adjusting maximum_distance_mm field in "config.xml" to set the far clipping plane. However, a better solution is to first recover a depth map for the "background" scene, then place your object into the scene. Only the closest point along each camera ray will be retained, with similar depths rejected. This simple depth-based background segmentation will also remove background pixels from the final point cloud. To use background segmentation, first select 'B' from the main menu. Afterwards, all scans (run with 'S') will use this background. Select 'R' to reset the background (i.e., to save the depth for all pixels with sufficient contrast).

----------------------
Compiling the Software
----------------------

These directions describe how to configure and use the provided software in Windows XP using Microsoft Visual Studio 2005 (with Visual C++ 2005). This document will be updated in the future to describe similar procedures for other development environments.

Begin by installing OpenCV 1.1pre1 from http://sourceforge.net/projects/opencvlibrary/. Follow the directions at http://opencv.willowgarage.com/wiki/VisualC%2B%2B to setup OpenCV for use with Visual Studio. Afterwards, open "cvStructuredLight.sln" in Visual Studio. If OpenCV is properly configured, you should not have any compilation errors.

-------------------
Release Information
-------------------

This software was originally developed by Douglas Lanman at Brown University in June of 2009. Brief revision notes follow.

   - Version 1.0 (1 July 2009)
     > First release supporting projector-camera calibration and reconstruction.

----------------
Acknowledgements
----------------

Gabriel Taubin originally conceived and co-developed the "Build your Own 3D  Scanner" course, as presented at SIGGRAPH 2009. In addition, Matt Hirsch, Ankit Mohan, and Ramesh Raskar provided useful comments that influenced the design of the software provided in that course, including this release. Matt Hirsch further developed a Linux port of this software, which should be made available on the course website sometime in August of 2009.

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