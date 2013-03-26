
% DISPLAYCALIB Visualizes the camera and projector calibration.
%    DISPLAYCALIB Displays the extrinsic calibration for the camera and 
%    the projector, using printed/projected checkerboards.
%     
%    This visualization code is based on the "ext_calib.m" function from 
%    the "Camera Calibration Toolbox for Matlab". Please consult that 
%    function for additional documentation of plotting functions.
%
%    Please read the SIGGRAPH 2009 course notes for additional details.
%
%       Douglas Lanman and Gabriel Taubin
%       "Build Your Own 3D Scanner: 3D Photography for Beginners"
%       ACM SIGGRAPH 2009 Course Notes    
%
% Douglas Lanman
% Brown University
% June 2009

% Reset Matlab environment.
clear all; clc;
addpath('./xml_io_tools');

% Define calibration parameters.
calibDir = 'C:\Documents and Settings\Douglas\Desktop\Active Projects\3D Scanning (SIGGRAPH 2009 Course)\Source\Doug\OpenCV\cvStructuredLight\Debug\output\calib\proj\';

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Define projector-camera system parameters.

% Projector and camera resolution.
proj_w         = 1024; % projector columns
proj_h         = 768;  % projector rows
cam_w          = 960;  % camera columns
cam_h          = 720;  % camera rows

% Printed chessboard parameters.
cam_board_w    = 8;    % interior camera chessboard corners (along width)
cam_board_h    = 6;    % interior camera chessboard corners (along height)
cam_board_w_mm = 30.0; % physical length of chessboard square (width in mm)
cam_board_h_mm = 30.0; % physical length of chessboard square (height in mm)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load projector-camera calibration parameters from OpenCV.

% Display progress.
disp('[Visualize Projector-Camera Calibration]');
disp('> Loading parameters from OpenCV session...');

% Load calibration results.
cam_intrinsic   = opencv_read([calibDir 'cam_intrinsic.xml']);
cam_distortion  = opencv_read([calibDir 'cam_distortion.xml']);
cam_extrinsic   = opencv_read([calibDir 'cam_extrinsic.xml']);
proj_intrinsic  = opencv_read([calibDir 'proj_intrinsic.xml']);
proj_distortion = opencv_read([calibDir 'proj_distortion.xml']);
proj_extrinsic  = opencv_read([calibDir 'proj_extrinsic.xml']);

% Convert extrinsic rotations vectors to matrices.
% Note: Assumes "Camera Calibration Toolbox for Matlab" is installed.
cam_rotation_matrix_00  = rodrigues(cam_extrinsic(1,:));
proj_rotation_matrix_00 = rodrigues(proj_extrinsic(1,:));

% Extract extrinsic translation vectors.
cam_translation_vector_00  = cam_extrinsic(2,:);
proj_translation_vector_00 = proj_extrinsic(2,:);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Visualize extrinsic calibration using "Camera Calibration Toolbox for Matlab".

% Display progress.
disp('> Displaying extrinsic calibration; press any key to continue');

% Create figure window.
figure(1); clf;
set(gcf,'Name','Projector-Camera Calibration');
set(gcf,'Renderer','OpenGL');

% Define camera calibration data structures.
nx_cam{1} = cam_w;
ny_cam{1} = cam_h;
dX_cam{1} = cam_board_w_mm;
fc_cam{1} = [cam_intrinsic(1,1) cam_intrinsic(2,2)];
alpha_c_cam{1} = 0;
cc_cam{1} = cam_intrinsic(1:2,3)';
Rc_1_cam{1} = cam_rotation_matrix_00;
Tc_1_cam{1} = cam_translation_vector_00';

% Define projector calibration data structures.
nx_proj = proj_w;
ny_proj = proj_h;
dX_proj = dX_cam{1};
fc_proj = [proj_intrinsic(1,1) proj_intrinsic(2,2)];
alpha_c_proj = 0;
cc_proj = proj_intrinsic(1:2,3)';
Rc_1_proj = proj_rotation_matrix_00;
Tc_1_proj = proj_translation_vector_00';

% Plot camera calibration.
for camIdx = 1:length(fc_cam)
   IP = 5*dX_cam{camIdx}*[1 -alpha_c_cam{camIdx} 0; 0 1 0;0 0 1]*...
        [1/fc_cam{camIdx}(1) 0 0; 0 1/fc_cam{camIdx}(2) 0; 0 0 1]*...
        [1 0 -cc_cam{camIdx}(1); 0 1 -cc_cam{camIdx}(2); 0 0 1]*...
        [0 nx_cam{camIdx}-1 nx_cam{camIdx}-1 0 0; 0 0 ny_cam{camIdx}-1 ny_cam{camIdx}-1 0; 1 1 1 1 1];
   BASE = 5*dX_cam{camIdx}*[0 1 0 0 0 0; 0 0 0 1 0 0; 0 0 0 0 0 1];
   IP = reshape([IP; BASE(:,1)*ones(1,5); IP],3,15);
   hold on;
      BASE = Rc_1_cam{1}*Rc_1_cam{camIdx}'*(BASE - Tc_1_cam{camIdx}*ones(1,6)) + Tc_1_cam{1}*ones(1,6);
      IP   = Rc_1_cam{1}*Rc_1_cam{camIdx}'*(IP - Tc_1_cam{camIdx}*ones(1,15)) + Tc_1_cam{1}*ones(1,15);
      plot3(BASE(1,:),BASE(3,:),-BASE(2,:),'b-','linewidth',2);
      plot3(IP(1,:),IP(3,:),-IP(2,:),'r-','linewidth',2);
      u = [6*dX_cam{camIdx} 0 0; -dX_cam{camIdx} 0 5*dX_cam{camIdx}; ...
           0 6*dX_cam{camIdx} 0; -dX_cam{camIdx} -dX_cam{camIdx} -dX_cam{camIdx}]';
      u = Rc_1_cam{1}*Rc_1_cam{camIdx}'*(u - Tc_1_cam{camIdx}*ones(1,4)) + Tc_1_cam{1}*ones(1,4);
      text(u(1,1),u(3,1),-u(2,1),['X_{c',int2str(camIdx),'}']);
      text(u(1,2),u(3,2),-u(2,2),['Z_{c',int2str(camIdx),'}']);
      text(u(1,3),u(3,3),-u(2,3),['Y_{c',int2str(camIdx),'}']);
      text(u(1,4),u(3,4),-u(2,4),['O_{c',int2str(camIdx),'}']);
   hold off;
end

% Plot projector calibration results (in camera coordinate system).
IP = 5*dX_proj*[1 -alpha_c_proj 0; 0 1 0;0 0 1]*...
     [1/fc_proj(1) 0 0; 0 1/fc_proj(2) 0; 0 0 1]*...
     [1 0 -cc_proj(1); 0 1 -cc_proj(2); 0 0 1]*...
     [0 nx_proj-1 nx_proj-1 0 0; 0 0 ny_proj-1 ny_proj-1 0; 1 1 1 1 1];
BASE = 5*dX_proj*[0 1 0 0 0 0; 0 0 0 1 0 0; 0 0 0 0 0 1];
IP = reshape([IP; BASE(:,1)*ones(1,5); IP],3,15);
BASE = Rc_1_cam{1}*Rc_1_proj'*(BASE - Tc_1_proj*ones(1,6)) + Tc_1_cam{1}*ones(1,6);
IP = Rc_1_cam{1}*Rc_1_proj'*(IP - Tc_1_proj*ones(1,15)) + Tc_1_cam{1}*ones(1,15);
hold on;
  plot3(BASE(1,:),BASE(3,:),-BASE(2,:),'b-','linewidth',2);
  plot3(IP(1,:),IP(3,:),-IP(2,:),'g-','linewidth',2);
  u = [6*dX_proj 0 0; -dX_proj 0 5*dX_proj; ...
       0 6*dX_proj 0; -dX_proj -dX_proj -dX_proj]';
  u = Rc_1_cam{1}*Rc_1_proj'*(u - Tc_1_proj*ones(1,4)) + Tc_1_cam{1}*ones(1,4);
  text(u(1,1),u(3,1),-u(2,1),'X_p');
  text(u(1,2),u(3,2),-u(2,2),'Z_p');
  text(u(1,3),u(3,3),-u(2,3),'Y_p');
  text(u(1,4),u(3,4),-u(2,4),'O_p');
hold off;

% Display distance between camera and projector centers.
C_cam  = Rc_1_cam{1}*(-Rc_1_cam{1}'*Tc_1_cam{1})+Tc_1_cam{1};
C_proj = Rc_1_cam{1}*(-Rc_1_proj'*Tc_1_proj)+Tc_1_cam{1};
baseline = (C_proj-C_cam)/25.4;
disp(['  <Projector-camera baseline = [',...
   num2str(baseline(1),'%0.1f'),', ',...
   num2str(baseline(2),'%0.1f'),', ',...
   num2str(baseline(3),'%0.1f'),...
   '] inches (in camera coords)>']);

% Plot the camera calibration board.
nx = cam_board_w;
ny = cam_board_h;
dX = cam_board_w_mm;
dY = cam_board_h_mm;
[X Y] = meshgrid(dY*(-1:ny),dX*(-1:nx));
P = [X(:) Y(:) zeros(length(X(:)),1)]';
PP = Rc_1_cam{1}*P + repmat(Tc_1_cam{1},1,length(X(:)));
XP = reshape(PP(1,:),[nx+2,ny+2]);
YP = reshape(PP(2,:),[nx+2,ny+2]);
ZP = reshape(PP(3,:),[nx+2,ny+2]);
hold on;
   for r = 1:2:(nx+1)
      for c = 1:2:(ny+1)
         patch( [XP(r,c) XP(r,c+1) XP(r+1,c+1) XP(r+1,c)],...
                [ZP(r,c) ZP(r,c+1) ZP(r+1,c+1) ZP(r+1,c)],...
               -[YP(r,c) YP(r,c+1) YP(r+1,c+1) YP(r+1,c)],...
               [0 0 0],'FaceAlpha',0.5);
      end
   end
   for r = 2:2:(nx+1)
      for c = 2:2:(ny+1)
         patch( [XP(r,c) XP(r,c+1) XP(r+1,c+1) XP(r+1,c)],...
                [ZP(r,c) ZP(r,c+1) ZP(r+1,c+1) ZP(r+1,c)],...
               -[YP(r,c) YP(r,c+1) YP(r+1,c+1) YP(r+1,c)],...
               [0 0 0],'FaceAlpha',0.5);
      end
   end
   plot3( [XP(1,1) XP(1,end) XP(end,end) XP(end,1) XP(1,1)],...
          [ZP(1,1) ZP(1,end) ZP(end,end) ZP(end,1) ZP(1,1)],...
         -[YP(1,1) YP(1,end) YP(end,end) YP(end,1) YP(1,1)],'k-');
hold off;

% Update plot properties.
xlabel('X_{c1}'); ylabel('Z_{c1}'); zlabel('Y_{c1}');
view(50,20); grid on; rotate3d on;
axis equal tight vis3d;
cameratoolbar; cameratoolbar('SetCoordSys','none');
pause;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Visualize lens distortions using "Camera Calibration Toolbox for Matlab".

% Display camera lens distortion.
nx = cam_w;
ny = cam_h;
fc = [cam_intrinsic(1,1) cam_intrinsic(2,2)];
alpha_c = 0;
cc = cam_intrinsic(1:2,3)';
kc = cam_distortion';
fc_error = zeros(1,2);
alpha_c_error = 0;
cc_error = zeros(1,2);
kc_error = zeros(1,5);
err_std = zeros(1,2);
visualize_distortions;

% Display status.
disp('> Displaying camera lens distortion; press any key to continue.');
pause;

% Display projector lens distortion.
nx = proj_w;
ny = proj_h;
fc = [proj_intrinsic(1,1) proj_intrinsic(2,2)];
alpha_c = 0;
cc = proj_intrinsic(1:2,3)';
kc = proj_distortion';
fc_error = zeros(1,2);
alpha_c_error = 0;
cc_error = zeros(1,2);
kc_error = zeros(1,5);
err_std = zeros(1,2);
visualize_distortions;

% Display status.
disp('> Displaying projector lens distortion; press any key to continue.');
pause;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Finalize visualization.

% Close figures and exit.
close all;
disp(' ');
