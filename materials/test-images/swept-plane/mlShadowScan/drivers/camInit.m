function camera = camInit(camName,camID,camFormat)

% CAMINIT Initializes camera(s).
%    CAMINIT initializes camera(s) supported by the Matlab
%    Image Acquisition Toolbox.
%    
%   Refer to SIGGRAPH 2009 course notes for additional details.
%
%       D. Lanman and G. Taubin, "Build Your Own 3D Scanner: 3D 
%       Photography for Beginners", ACM SIGGRAPH 2009 Course 
%       Notes, 2009.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 28 May 2009

% Initialize camera(s) using Image Acquisition Toolbox.
nCameras = length(camName);
camera = cell(1,nCameras);
for i = 1:nCameras
   camera{i} = videoinput(camName{i},camID(i),camFormat{i});
   set(camera{i},'ReturnedColorSpace','rgb');
   triggerconfig(camera{i},'manual');
   set(camera{i},'FramesPerTrigger',1);
   set(camera{i},'TriggerRepeat',128);
   src = getselectedsource(camera{i});
   %set(src,'BacklightCompensation','off');
   start(camera{i});
end

% % Grab some frames to allow gain to stabilize.
% for i = 1:10
%    camCapture(camera);
% end