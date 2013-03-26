
% Reset Matlab environment.
clear; clc;

% Add required subdirectories.
addpath('../../utilities');

% Select projector calibation images for inclusion.
useProjImages = 1:20; %[1:5,7:9];

% Define projector calibration plane properties.
% Note: These lengths are used to determine the extrinsic calibration.
%       That is, the transformation(s) from reference plane(s) -> camera.
proj_cal_nX = 1;         % number of rectangles along x-axis
proj_cal_nY = 1;         % number of rectangles along y-axis
proj_cal_dX = 406.4;     % length along x-axis (mm) (between checkerboard rectangles) [406.4]
proj_cal_dY = 335.0;     % length along y-axis (mm) (between checkerboard rectangles) [330.2]


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part I: Calibration of the camera(s) independently of the projector.

% Load all checkerboard corners for calibration images (for each camera).
% Note: Must select same corners in first images so that world coordinate
%       system is consistent across all cameras.
camIndex = 1;
while exist(['./cam/v',int2str(camIndex)],'dir')
   
   % Load checkerboard corners for calibration images.
   load(['./cam/v',int2str(camIndex),'/calib_data']);

   % Display the first camera calibration image.
   I_cam1 = imread(['./cam/v',int2str(camIndex),'/001.bmp']);
   figure(1); image(I_cam1); axis image;
   hold on;
      plot(x_1(1,:)+1,x_1(2,:)+1,'r+');
   hold off;
   title('First camera calibration image: 001.bmp');
   drawnow;

   % Configure the camera model (must be done before calibration).
   est_dist     = [1;1;1;1;0]; % select distortion model
   est_alpha    = 0;           % include skew
   center_optim = 1;           % estimate the principal point

   % Run the main calibration routine.
   go_calib_optim;

   % Save the camera calibration results as camera_results.mat.
   saving_calib; 
   copyfile('Calib_Results.mat',['./calib_results/camera_results_v',...
      int2str(camIndex),'.mat']);
   delete('Calib_Results.mat'); delete('Calib_Results.m');

   % Save the camera calibration results (in "side" variables).
   fc_cam{camIndex}       = fc;
   cc_cam{camIndex}       = cc;
   kc_cam{camIndex}       = kc;
   alpha_c_cam{camIndex}  = alpha_c;
   Rc_1_cam{camIndex}     = Rc_1;
   Tc_1_cam{camIndex}     = Tc_1;
   x_1_cam{camIndex}      = x_1;
   X_1_cam{camIndex}      = X_1;
   nx_cam{camIndex}       = nx;
   ny_cam{camIndex}       = ny;
   n_sq_x_1_cam{camIndex} = n_sq_x_1;
   n_sq_y_1_cam{camIndex} = n_sq_y_1;
   dX_cam{camIndex}       = dX;
   dY_cam{camIndex}       = dY;
   
   % Increment camera index.
   camIndex = camIndex+1;
   
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part II: Projector-Camera Color Calibration (if necessary).

% Obtain color calibration matrix for each camera.
camIndex = 1; bounds = {}; projValue = {}; inv_color_calib_cam = {};
while exist(['./cam/v',int2str(camIndex)],'dir')
   
   % Load all color calibration images for this camera.
   C = {};
   for j = 1:3
      C{j} = imread(['./cam/v',int2str(camIndex),'/c',int2str(j),'.bmp']);
      projValue{camIndex,j} = load(['./cam/v',int2str(camIndex),'/projValue.mat']);
   end
   
   % Select/save bounding box(if none exists).
   color_bounds = {};
   if ~exist(['./cam/v',int2str(camIndex),'/color_bounds.mat'],'file')
      for j = 1:3
         figure(10); clf;
         imagesc(C{j}); axis image off;
         title('Select the bounding box for color calibration'); drawnow;
         [x,y] = ginput(2);
         color_bounds{j} = round([min(x) max(x) min(y) max(y)]);
         hold on;
            plot(color_bounds{j}([1 2 2 1 1]),color_bounds{j}([3 3 4 4 3]),'r.-'); 
         hold off;
         pause(0.3);
      end
      bounds{camIndex} = color_bounds;
      save(['./cam/v',int2str(camIndex),'/color_bounds.mat'],'color_bounds');
   else
      load(['./cam/v',int2str(camIndex),'/color_bounds.mat']);
      bounds{camIndex} = color_bounds;
   end
   
   % Extract projector-camera color correspondences.
   cam_colors = []; proj_colors = [];
   for j = 1:3
      crop_C = C{j}(bounds{camIndex}{j}(3):bounds{camIndex}{j}(4),...
                   bounds{camIndex}{j}(1):bounds{camIndex}{j}(2),:);
%       imagesc(crop_C);
%       axis image; pause;
      crop_C = double(reshape(crop_C,[],3,1)');
      crop_P = zeros(size(crop_C));
      cam_colors = [cam_colors crop_C];
      if j == 1
         crop_P(1,:) = projValue{camIndex,j}.projValue;
      elseif j == 2
         crop_P(2,:) = projValue{camIndex,j}.projValue;
      else
         crop_P(3,:) = projValue{camIndex,j}.projValue;
      end
      proj_colors = [proj_colors crop_P];
   end
   inv_color_calib_cam{camIndex} = inv(cam_colors/proj_colors);
   figure(11); 
      plot([cam_colors(:,1:1e3:end); proj_colors(:,1:1e3:end)]'); grid on;
   figure(12); drawnow;
      plot([inv_color_calib_cam{camIndex}*cam_colors(:,1:1e3:end); ...
         proj_colors(:,1:1e3:end)]'); grid on; drawnow;
   
   % Increment camera index.
   camIndex = camIndex+1;
   
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part III: Calibration of the projector (using first camera calibration).

% Load all projected checkerboard corners for calibration images.
% Note: Calibrate reference plane positions (if not available).
load('./proj/v1/calib_data');
if ~exist('./proj/v1/ext_calib_data.mat','file')
   load('./proj/v1/calib_data','n_ima')
   for i = 1:n_ima
      frame = ['./proj/v1/',num2str(i,'%0.3d')];
      [Tc,Rc,H] = ...
         computeExtrinsic(fc_cam{1},cc_cam{1},kc_cam{1},alpha_c_cam{1},frame,'bmp',...
                          proj_cal_nX,proj_cal_nY,proj_cal_dX,proj_cal_dY);
      eval(['Rc_',int2str(i),' = Rc']);
      eval(['Tc_',int2str(i),' = Tc']);
      if i == 1
         save('./proj/v1/ext_calib_data.mat',['Rc_',int2str(i)]);
      else
         save('./proj/v1/ext_calib_data.mat',['Rc_',int2str(i)],'-append');
      end
      save('./proj/v1/ext_calib_data.mat',['Tc_',int2str(i)],'-append');
   end
else
   load('./proj/v1/ext_calib_data');
end
for i = 1:n_ima
   eval(['xproj_',int2str(i),' = x_',int2str(i),';']);
end

% Synthesize projector image coordinates.
% Note: This assumes that 7x7 checkerboard blocks were selected in
%       each image (i.e., row-by-col). In addition, this assumes that
%       selection was counter-clockwise starting in the lower left.
%       Finally, we assume that each square is 64x64 pixels.
%       Checkerboard begins 1x1 square "in" and x_i is the ith
%       points from left-to-right and top-to-bottom.
I = 255*ones(768,1024,'uint8');
C = uint8(255*(checkerboard(64,4,4) > 0.5));
I((1:size(C,1))+(2*64)*1,(1:size(C,2))+(2*64)*2) = C;
x = (0:64:64*6)+(2*64)*2+64+0.5-1;
y = (0:64:64*6)+(2*64)*1+64+0.5-1;
[X,Y] = meshgrid(x,y);
X = X'; Y = Y';
for i = 1:n_ima
   eval(['x_proj_',int2str(i),' = [X(:) Y(:)]'';']);
end

% Display the first projector calibration image.
I_proj1 = imread('./proj/v1/001.bmp');
figure(2); image(I_proj1);
hold on;
   plot(xproj_1(1,:)+1,xproj_1(2,:)+1,'r+');
hold off;
title('First projector calibration image: 001.bmp');
drawnow;

% Display points on projector image.
figure(3); image(I); colormap(gray);
hold on;
   plot(x_proj_1(1,:)+1,x_proj_1(2,:)+1,'r.');
hold off;
title('Points overlaid on projector image');
drawnow;

% Display the 3D configuraton using the extrinsic calibration.
X_proj     = []; % 3D coordinates of the points
x_proj     = []; % 2D coordinates of the points in the projector image
n_ima_proj = []; % number of calibration images
for i = useProjImages
   
   % Transform calibation marker positions to camera coordinate system.
   X = [0 proj_cal_dX proj_cal_dX 0; 0 0 proj_cal_dY proj_cal_dY; 0 0 0 0];
   X = eval(['Rc_',int2str(i),'*X + repmat(Tc_',int2str(i),',1,size(X,2));']);
   
   % Recover 3D points corresponding to projector calibration pattern.
   % Note: Uses ray-plane intersection to solve for positions.
   V = eval(['pixel2ray(xproj_',int2str(i),',fc_cam{1},cc_cam{1},kc_cam{1},alpha_c_cam{1});']);
   C = zeros(3,size(V,2));
   calPlane = fitPlane(X(1,:),X(2,:),X(3,:));
   X = intersectLineWithPlane(C,V,calPlane');
   
   % Concatenate projector and world correspondances.
   eval(['xproj = xproj_',num2str(i),';']);
   eval(['X_proj_',num2str(i),' = X;']);
   eval(['X_proj = [X_proj X_proj_',num2str(i),'];']);
   eval(['x_proj = [x_proj x_proj_',num2str(i),'];']);
   n_ima_proj = [n_ima_proj i*ones(1,size(xproj,2))];
   
end

% Configure the projector model (must be done before calibration).
nx           = 1024;        % number of projector columns
ny           = 768;         % number of projector rows
no_image     = 1;           % only use pre-determined corners
n_ima        = 1;           % default behavior for a single image
est_dist     = [1;1;0;0;0]; % select distortion model
est_alpha    = 0;           % no skew
center_optim = 1;           % estimate the principal point

% Run the main projector calibration routine.
X_1 = X_proj;
x_1 = x_proj;
go_calib_optim;

% Save the projector calibration results as projector_results.mat.
saving_calib; 
copyfile('Calib_Results.mat','./calib_results/projector_results.mat');
delete('Calib_Results.mat'); delete('Calib_Results.m');

% Save the projector calibration results (in "side" variables).
fc_proj       = fc;
cc_proj       = cc;
kc_proj       = kc;
alpha_c_proj  = alpha_c;
Rc_1_proj     = Rc_1;
Tc_1_proj     = Tc_1;
x_1_proj      = eval(['x_proj_',num2str(useProjImages(1)),';']);
X_1_proj      = eval(['X_proj_',num2str(useProjImages(1)),';']);
nx_proj       = nx;
ny_proj       = ny;
n_sq_x_1_proj = eval(['n_sq_x_',num2str(useProjImages(1)),';']);
n_sq_y_1_proj = eval(['n_sq_y_',num2str(useProjImages(1)),';']);
dX_proj       = dX;
dY_proj       = dY;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part IV: Display calibration results.

% Plot camera calibration results (in camera coordinate system).
procamCalibDisplay;
for i = 1
   hold on;
      Y     = Rc_1_cam{i}*X_1_cam{i} + Tc_1_cam{i}*ones(1,size(X_1_cam{i},2));
      Yx    = zeros(n_sq_x_1_cam{i}+1,n_sq_y_1_cam{i}+1);
      Yy    = zeros(n_sq_x_1_cam{i}+1,n_sq_y_1_cam{i}+1);
      Yz    = zeros(n_sq_x_1_cam{i}+1,n_sq_y_1_cam{i}+1);
      Yx(:) = Y(1,:); Yy(:) = Y(2,:); Yz(:) = Y(3,:);
      mesh(Yx,Yz,-Yy,'EdgeColor','r','LineWidth',1);
      Y     = X_1_proj;
      Yx    = zeros(n_sq_x_1_proj+1,n_sq_y_1_proj+1);
      Yy    = zeros(n_sq_x_1_proj+1,n_sq_y_1_proj+1);
      Yz    = zeros(n_sq_x_1_proj+1,n_sq_y_1_proj+1);
      Yx(:) = Y(1,:); Yy(:) = Y(2,:); Yz(:) = Y(3,:);
      mesh(Yx,Yz,-Yy,'EdgeColor','g','LineWidth',1);
   hold off;
end
title('Projector/Camera Calibration Results');
xlabel('X_c'); ylabel('Z_c'); zlabel('Y_c');
view(50,20); grid on; rotate3d on;
axis equal tight vis3d; drawnow;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part V: Save calibration results.

% Determine mapping from projector pixels to optical rays.
% Note: Ideally, the projected images should be pre-warped to
%       ensure that projected planes are actually planar.
c = 1:nx_proj;
r = 1:ny_proj;
[C,R] = meshgrid(c,r);
np  = pixel2ray([C(:) R(:)]',fc_proj,cc_proj,kc_proj,alpha_c_proj);
np = Rc_1_proj'*(np - Tc_1_proj*ones(1,size(np,2)));
Np = zeros([ny_proj nx_proj 3]);
Np(:,:,1) = reshape(np(1,:),ny_proj,nx_proj);
Np(:,:,2) = reshape(np(2,:),ny_proj,nx_proj);
Np(:,:,3) = reshape(np(3,:),ny_proj,nx_proj);
P = -Rc_1_proj'*Tc_1_proj;

% Estimate plane equations describing every projector column.
% Note: Resulting coefficient vector is in camera coordinates.
wPlaneCol = zeros(nx_proj,4);
for i = 1:nx_proj
   wPlaneCol(i,:) = ...
      fitPlane([P(1); Np(:,i,1)],[P(2); Np(:,i,2)],[P(3); Np(:,i,3)]);
   %figure(4); hold on;
   %plot3(Np(:,i,1),Np(:,i,3),-Np(:,i,2),'r-');
   %drawnow;
end

% Estimate plane equations describing every projector row.
% Note: Resulting coefficient vector is in camera coordinates.
wPlaneRow = zeros(ny_proj,4);
for i = 1:ny_proj
   wPlaneRow(i,:) = ...
      fitPlane([P(1) Np(i,:,1)],[P(2) Np(i,:,2)],[P(3) Np(i,:,3)]);
   %figure(4); hold on;
   %plot3(Np(i,:,1),Np(i,:,3),-Np(i,:,2),'g-');
   %drawnow;
end

% Pre-compute optical rays for each camera pixel.
for i = 1:length(fc_cam)
   c = 1:nx_cam{i};
   r = 1:ny_cam{i};
   [C,R] = meshgrid(c,r);
   Nc{i} = Rc_1_cam{1}*Rc_1_cam{i}'*pixel2ray([C(:) R(:)]'-1,fc_cam{i},cc_cam{i},kc_cam{i},alpha_c_cam{i});
   Oc{i} = Rc_1_cam{1}*Rc_1_cam{i}'*(-Tc_1_cam{i}) + Tc_1_cam{1};
end

% Save the projector/camera calibration results as calib_cam_proj.mat.
save_command = ...
   ['save ./calib_results/calib_cam_proj ',...
    'fc_cam cc_cam kc_cam alpha_c_cam Rc_1_cam Tc_1_cam ',...
    'x_1_cam X_1_cam nx_cam ny_cam n_sq_x_1_cam n_sq_y_1_cam ',...
    'dX_cam dY_cam ',...
    'inv_color_calib_cam ',...
    'fc_proj cc_proj kc_proj alpha_c_proj Rc_1_proj Tc_1_proj ',...
    'x_1_proj X_1_proj nx_proj ny_proj n_sq_x_1_proj n_sq_y_1_proj '...
    'dX_proj dY_proj '...
    'Oc Nc wPlaneCol wPlaneRow'];
eval(save_command);
