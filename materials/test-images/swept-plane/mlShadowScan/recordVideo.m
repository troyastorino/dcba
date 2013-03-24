
% RECORDVIDEO Implements video capture for "Scanning with Shadows".
%    RECORDVIDEO captures video using the Image Acquistion Toolbox
%    for Matlab. The output image sequences can be used with
%    SHADOWSCAN, which implements an inexpensive 3D photography 
%    system using an approach first proposed by Bouguet and Perona.
%
%       J.-Y. Bouguet and P. Perona, "3D photography using shadows
%       in dual-space geometry", International Journal of Computer
%       Vision (IJCV), 1999.
%     
%    Please read the SIGGRAPH 2009 course notes for more details.
%
%       D. Lanman and G. Taubin, "Build Your Own 3D Scanner: 3D 
%       Photography for Beginners", ACM SIGGRAPH 2009 Course 
%       Notes, 2009.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009

% Reset Matlab environment.
clear; clc;

% Add required subdirectories to the path.
addpath('./drivers');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 1: Define the video capture parameters.                                        

% Specify the object parameters.
objName = 'demo';    % object name (should correspond to a data dir.)
seqName = 'v1';      % sequence name (subdirectory of object)

% Define camera for use with the Image Acquistion Toolbox.
% Note: Type 'imaqhwinfo' at comment prompt for camera properties.
%       For example, 'info = imaqhwinfo('winvideo');'.
camName     = {'winvideo'};
camID       = 1;
camFormat   = {'RGB24_1600x1200'};
camRes      = [720 960];


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 2: Capture video sequence.

% Create output directory (empty contents if directory already exists).
disp('[Video Capture for Scanning with Shadows]');
disp(' '); disp('Creating output directories...');
outDir = ['./data/',objName,'/',seqName,'/'];
if ~exist(['./data/',objName],'dir')
   mkdir(['./data/',objName]);
end
if exist(outDir,'dir')
   rmdir(outDir,'s');
end
mkdir(outDir);

% Initialize camera.
% Note: Make sure to optimize the camera settings.
if ~exist('camera','var')
   disp('Initializing video capture...');
   camera = imaqfind('Type','videoinput');
   for i = 1:length(camera)
      delete(camera{i});
   end
   camera = camInit(camName,camID,camFormat);
end

% Create video capture interface.
disp('Capturing video sequence...');
disp('+ Use start/stop button in figure to record frames...');
figure(1); clf; 
set(gcf,'Name','Video Capture for Scanning with Shadows');
set(gcf,'MenuBar','none');
I = camCapture(camera);
I = imresize(I{1},camRes);
set(gca,'Pos',[0 0 1 1]);
hImage = imagesc(I);
axis image off;

% Create start/stop pushbutton.
startCapture = false;
stopCapture  = false;
uiButton = uicontrol('Style','pushbutton',...
   'Units', 'normalized',...
   'Position',[0.015 0.0175 0.1 0.05],...
   'Value',1,'String','Start',...
   'Callback',...
      ['disp(''+ Saving frames...'');',...
       'startCapture = true;',...
       'set(uiButton,''String'',''Stop'');',...
       'set(uiButton,''Callback'',''stopCapture = true;'');']);

% Capture video sequence.
frameIdx = 1;
while ~stopCapture 
   I = camCapture(camera);
   I = imresize(I{1},camRes); 
   set(hImage,'Cdata',I);
   if startCapture
      imwrite(I,[outDir,num2str(frameIdx,'%0.6d'),'.jpg'],'Quality',100);
      frameIdx = frameIdx + 1;
   end
end

% Finalize video capture.
disp('Finalizing video capture...');
if exist('camera','var')
   camStop(camera);
end
camera = imaqfind('Type','videoinput');
for i = 1:length(camera)
   delete(camera{i});
end
close all;

