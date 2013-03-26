
% EXTRINSICDEMO Demonstrates how to find extrinsic calibration.
%    EXTRINSICDEMO can be used to obtain and visualize the
%    extrinsic calibration for the "desktop scanning" configuration,
%    as presented in:
%
%       J.-Y. Bouguet and P. Perona, "3D photography using shadows
%       in dual-space geometry", International Journal of Computer
%       Vision (IJCV), 1999.
%     
%    Please read the accompanying course notes handout for more
%    detailed descriptions of the included support code.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009

% Reset Matlab environment.
clear; clc;

% Specify the demonstration parameters.
objName    = 'man';        % object name (should correspond to a data dir.)
seqName    = 'v1';         % sequence name (subdirectory of object)
calName    = 'calib';      % calibration source (also must be a data dir.)
useLowRes  = true;         % enables low resolution imagery (for debugging)

% Define "horizonal" and "vertical" reference plane properties.
% Note: These lengths are used to determine the extrinsic calibration.
%       That is, the transformation(s) from reference plane(s) -> camera.
nX = 1;        % number of rectangles along x-axis
nY = 1;        % number of rectangles along y-axis
dX = 558.8;    % length along x-axis (mm) (between checkerboard rectangles)
dY = 303.2125; % length along y-axis (mm) (between checkerboard rectangles)


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 1: Extrinsic Calibration.

% Display progress.
disp('[Extrinsic Demonstration]');
disp(' ');
disp('Determining extrinsic calibration of reference plane(s)...');

% Modify extensions for low-resolution imagery (if enabled).
if useLowRes
   seqName = [seqName,'-lr'];
   calName = [calName,'-lr'];
end

% Load intrinsic calibration.
% Note: This assumes that you have already completed intrinsic calibration
%       for this sequence. See the assignment handout for more details.
load(['data/',calName,'/Calib_Results'],...
   'fc','cc','kc','alpha_c','nx','ny');

% Obtain the extrinsic calibration for the "horizontal" plane.
disp('   + finding the extrinsic parameters of the "horizontal" plane...');
firstFrame = ['data/',objName,'/',seqName,'/000001'];
[Tc_h,Rc_h,H_h] = ...
   computeExtrinsic(fc,cc,kc,alpha_c,firstFrame,'jpg',nX,nY,dX,dY);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 2: Visualization.

% Display progress.
disp('Displaying the extrinsic calibration...');

% Display the 3D configuraton using the extrinsic calibration.
% Note: Some of this is hard-coded for the configuration included
%       with the assignment. This could be generalized.
figure(2); set(gcf,'Name','Extrinsic Calibration'); clf;
X = [0 dX dX 0; 0 0 dY dY; 0 0 0 0];
patch(X(1,:),X(2,:),X(3,:),0.65*[1 1 1],'FaceAlpha',0.5);
hold on;
   % plots the "horizontal" plane
   plot3(X(1,:),X(2,:),X(3,:),'g.','MarkerSize',15); 
hold off;
hold on;
   % plots the camera center
   C = -Rc_h'*Tc_h;
   plot3(C(1),C(2),C(3),'b.','MarkerSize',20);
hold off;
axis equal tight; grid on; view(3); box on;
axis([-200 800 -1300 500 -100 1000]);
xlabel('x'); ylabel('y'); zlabel('z');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 3: Interactive Selection of Camera Rays.

% Display progress.
disp('Displaying user-selected pixels/rays...');

% Draw camera rays for user-selected pixel(s).
disp('   + select pixels with any mouse button, then press enter to continue...');
figure(1);
xlabel('select pixels with any mouse button, then press enter to continue');
x = ginput;
V = Rc_h'*pixel2ray(x',fc,cc,kc,alpha_c);
figure(2);
hold on;
   X = repmat(C,1,size(V,2));
   quiver3(X(1,:),X(2,:),X(3,:),V(1,:),V(2,:),V(3,:),2000,'r');
hold off;

% Return to Matlab prompt.
disp(' ');
