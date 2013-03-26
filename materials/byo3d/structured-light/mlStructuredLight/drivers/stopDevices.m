
% STOPDEVICES Stops projector(s) and camera(s).
%   Stops any valid 'Screen' objects, allowing fullscreen
%   projector control via the Psychtoolbox, and any valid
%   camera(s) supported by the Image Acquisition Toolbox. 
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

% Close the fullscreen display and camera(s).
if exist('Screen','var')
   clear Screen; Screen('CloseAll');
end
if exist('camera','var')
   camStop(camera);
end
camera = imaqfind('Type','videoinput');
for i = 1:length(camera)
   delete(camera{i});
end
clear all; clc;