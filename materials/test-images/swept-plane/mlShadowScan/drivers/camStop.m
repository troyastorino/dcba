function camStop(camera)

% CAMSTOP Stops active camera(s).
%    CAMSTOP stops camera(s) supported by the Matlab
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

% Close camera(s) using Image Acquisition Toolbox.
nCameras = length(camera);
for i = 1:nCameras
   delete(camera{i});
end