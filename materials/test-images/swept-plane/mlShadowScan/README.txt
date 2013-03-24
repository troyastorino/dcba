--------------------------------------------------------------------------------
3D Scanning using Swept-Planes
http://mesh.brown.edu/byo3d/index.html

Douglas Lanman
dlanman@gmail.com        
Brown University

Version 1.0 (18 May 2009)

--------
Overview
--------

Optical triangulation using swept-planes is a popular 3D scanning method, 
encompassing laser striping scanners. In fact, most commercial 3D scanners
use some form of laser striping. In this project we describe how to build an 
inexpensive, yet accurate, 3D scanner using household items and a digital 
camera. Specifically, we implement the “desktop scanner” originally proposed by
Bouguet and Perona (see below). Our instantiation of their system is composed 
of five primary items: a digital camera, a point-like light source, a stick, 
two planar surfaces, and a calibration checkerboard. By waving the stick in 
front of the light source, the user can cast planar shadows into the scene. As 
we demonstrate, the depth at each pixel can then be recovered using simple 
geometric reasoning.

This software implements the "desktop scanner" originally described in the
following publication:

   J.-Y. Bouguet and P. Perona, "3D photography using shadows
   in dual-space geometry", International Journal of Computer
   Vision (IJCV), 1999.

Our implementation uses Matlab and has the following main functions.

   - Manually-assisted intrinsic camera calibration (using the Matlab toolbox)
   - Manually-assisted extrinsic camera calibration (using the Matlab toolbox)
   - 3D scanning with swept-planes (specifically cast shadows)
   - Video capture using the Image Acquisition Toolbox for Matlab

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
	 Camera Calibration Toolbox for MATLAB

--------------------
Running the Software
--------------------

These directions describe how to configure and use the provided software. See
Chapter 4 of the course notes for additional instructions.

---------------------------------
> Step 1: Configuring your System

The first step is to assemble the "desktop scanner". You'll need a digital 
camera, a point-like light source, a stick, two planar surfaces, and a 
calibration checkerboard. A pair of blank white foamcore boards are used as 
planar calibration objects. These boards can be purchased at an art supply store. 
Any rigid light-colored planar object could be substituted, including particle 
board, acrylic sheets, or even lightweight poster board. At least four 
fiducials, such as printed checkerboard corners, should be affixed to known 
locations on each board. The distance and angle between each fiducial should be 
measured and recorded for later use in the calibration phase. The boards should 
be oriented approximately at a right angle to one another.

Next, a planar light source must be constructed. In this implementation we will 
follow the method of Bouguet and Perona, in which a point source and a stick are 
used to cast planar shadows. Wooden dowels of varying diameter can be obtained 
at a hardware store, and the point light source can be fashioned from any 
halogen desk lamp after removing the reflector. Alternatively, a laser stripe 
scanner could be implemented by replacing the point light source and stick with 
a modified laser pointer (although some software modifications would be required). 
In this case, a cylindrical lens must be affixed to the exit aperture of the 
laser pointer, creating a low-cost laser stripe projector. Both components can 
be obtained from Edmund Optics. For example, a section of a lenticular array or 
cylindrical Fresnel lens sheet could be used.

---------------------------------
> Step 2: Calibrating your Camera

The next step is to estimate the intrinsic calibration parameters of your 
camera. Begin by downloading and installing the Camera Calibration Toolbox for
Matlab, available at:

Camera Calibration Toolbox for Matlab
Jean-Yves Bouguet
http://www.vision.caltech.edu/bouguetj/calib_doc/

Begin by adding the toolbox to your MATLAB path by selecting 
“File -> Set Path...”. Next, change the current working directory to one of the 
calibration sequences (e.g., to the calib or calib-lr examples downloaded from
the course website). Type "calib" at the MATLAB prompt to start. Since we’re 
only using a few images, select “Standard (all the images are stored in memory)” 
when prompted. To load the images, select “Image names” and press return, then 
“j” (or a similar code for the captured image format). Now select “Extract grid 
corners”, pass through the prompts without entering any options, and then follow
 the on-screen directions. (Note that, for the provided examples, a calibration 
target with the default 30mm x 30mm squares was used). Always skip any prompts 
that appear, unless you are more familiar with the toolbox options. Once you’ve 
finished selecting corners, choose “Calibration”, which will run one pass though 
the calibration algorithm. Next, choose “Analyze error”. Left-click on any 
outliers you observe, then right-click to continue. Repeat the corner selection 
and calibration steps for any remaining outliers (this is a manually-assisted 
form of bundle adjustment). Once you have an evenly-distributed set of 
reprojection errors, select “Recomp. corners” and finally “Calibration”. To save
your intrinsic calibration, select “Save”.

------------------------------------------
> Step 3: Capturing a Swept-Plane Sequence

Any video camera or webcam can be used for image acquisition. The light source 
and camera should be separated, so that the angle between camera rays and cast 
shadow planes is close to perpendicular (otherwise triangulation will result in 
large errors). Data acquisition is simple. First, an object is placed on the 
horizontal calibration board, and the stick is slowly translated in front of the
light source. The stick should be waved such that a thin shadow slowly moves 
across the screen in one direction. Each point on the object should be shadowed 
at some point during data acquisition. Note that the camera frame rate will 
determine how fast the stick can be waved. If it is moved too fast, then some 
pixels will not be shadowed in any frame, leading to reconstruction artifacts.

Note that any video capture software can be used to record the input video 
sequence, as well as the still images required for camera calibration. One 
option is to capture the image sequence outside Matlab; for example, "Open 
Video Capture" is available for this purpose at:

Open Video Capture
008soft.com
http://www.008soft.com/products/video-capture.htm

Alternatively, the included "recordVideo" m-file can be used if the Image 
Acquistion Toolbox for Matlab is installed.

Place the scanning sequences into the data directory, with a descriptive name
(for example, see the frog and frog-lr examples provided on the course website).

----------------------------------------------------------
> Step 4: Reconstructing a Calibrated Swept-Plane Sequence

Reconstruction is accomplished using the provided "shadowScan" m-file. Begin by
opening this file in a text editor. The input sequence and various options are
defined in a scanning script (contained in the appropriate data subdirectory). 
For example, see "man_v1.m" in the data/man subdirectory. Note that the 
following options are defined (for example, see lines 2-7):

% Specify the object and reconstruction parameters.
objName   = 'man';     % object name (should correspond to a data dir.)
calName   = 'calib';   % calibration source (also must be a data dir.)
seqName   = 'v1';      % sequence name (subdirectory of object)
allFrames = 1:200;     % all frames in this sequence
recFrames = 61:151;    % frames used for reconstruction

Note that "allFrames" and "recFrames" define which frames will be processed. 
Update these lines to correspond with the sequence you recorded.

Next, various options must be defined (for example, see lines 9-13):

% Set reconstruction parameters.
useLowRes   = true;    % enables low resolution imagery (for debugging)
minContrast = 20;      % pixel exclusion threshold (min. contrast)
dSample     = 1;       % down-sampling rate for final point cloud
distReject  = 20;      % rejection distance (for outlier removal)

If the low-resolution flag is set, then the "*-lr" sequences will be used
(note you must create these sequences, or disable this flag). The minimum
contrast flag is used to determine whether a pixel has sufficient contrast to
accurately detect a shadow-crossing event. Finally, the "dSample" flag can be
set to only reconstruct a subset of the pixels in the image (i.e., skipping
every dSample pixels).

The remaining options are used to set the spacing of the fiducials on the 
calibration planes, as well as the clipping volume applied to the final point
cloud.

Once these parameters have been defined, simply type "shadowScan" at the
MATLAB prompt to begin scanning. Follow the on-screen directions to complete
the scanning process. The output point cloud will be saved in the models 
directory. Here we provide brief instructions describing each interaction. 
Note that the course slides provide figures/videos describing these steps.

[Scanning with Shadows]
 
Loading object and reconstruction parameters...
Performing video processing...
   + finding the dividing line between reference planes...
     (click on two points on the dividing line)

     > You should click on two points along the line dividing the horizontal
       calibration board from the vertical calibration board. This is used
       to automatically determine the endpoints of lines on the vertical
       and horizontal calibration planes.

   + define the reference area for the "vertical" plane...
     (click on top-left and bottom-right corners of reference area)
   + define the reference area for the "horizontal" plane...
     (click on top-left and bottom-right corners of reference area)

     > Here we are trying to define the "blank" area on the vertical and 
       and horizontal reference planes. In each case, click on the top-left
       and bottom-right corners of the "blank" area between the four fiducials
       on each plane which is completely unobstructed by the object or fiducials.
       This will be used to automatically determine the position of the shadow 
       boundaries cast on each calibration board.

   + estimating per-pixel dynamic range and shadow thresholds...
   + estimating shadow boundaries...
   + estimating shadow crossing time(s)...
   + displaying video processing results...
Determining extrinsic calibration of reference plane(s)...
   + finding the extrinsic parameters of the "horizontal" plane....
     (click on the four extreme corners of the rectangular pattern, 
      starting on the bottom-left and preceding counter-clockwise.)
   + finding the extrinsic parameters of the "vertical" plane....
     (click on the four extreme corners of the rectangular pattern, 
      starting on the bottom-left and preceding counter-clockwise.)

     > At this stage we need to define the extrinsic calibration of the 
       horizontal and vertical calibration planes (i.e., their position in the
       world coordinate system). This is done by simply clicking on the four 
       fiducials on each plane, starting on the bottom-left and preceding 
       counter-clockwise.
 
Reconstructing 3D points using intersection with shadow plane(s)...
   + recovering implicit representation of shadow planes...
   + reconstructing 3D points...
Display reconstruction results and exporting VRML file...
   + displaying reconstruction results...
   + exporting VRML file...

---------------------------------------------------
> Step 5: Visualizing the Reconstructed Point cloud

The point cloud is visualized automatically in MATLAB. In addition, an output
VRML file is created for use with the Java visualization software provided on
the course website. 

-------------------
Release Information
-------------------

This software was originally developed by Douglas Lanman at Brown University in 
May of 2009. Brief revision notes follow.

   - Version 1.0 (18 May 2009)
     > First release supporting swept-plane scanning using shadow-casting.

----------------
Acknowledgements
----------------

Gabriel Taubin conceived and co-developed the "Build your Own 3D Scanner" 
course, as presented at SIGGRAPH 2009. Thanks to Yong Zhao for assisting in data
collection and Daniel Crispell for useful discussion.

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