
% SHADOWSCAN Implements "Scanning with Shadows".
%    SHADOWSCAN implements an inexpensive 3D photography system
%    using an approach first proposed by Bouguet and Perona.
%
%       J.-Y. Bouguet and P. Perona, "3D photography using shadows
%       in dual-space geometry", International Journal of Computer
%       Vision (IJCV), 1999.
%     
%    This script can be used to reconstruct sequences generated
%    by our reference implementation. Please read the SIGGRAPH 
%    2009 course notes for additional details.
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
% Note: The "util" directory contains several useful subroutines,
%       such as least-squares line-fitting and line-plane intersection.
addpath(genpath('./util'));


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 1: Define the data capture and reconstruction parameters.                                        
% Note: Uncomment a block to reconstruct the corresponding sequence.

% Display progress.
disp('[Scanning with Shadows]');
disp(' ');
disp('Loading object and reconstruction parameters...');

% Select which reconstruction script to use.
%addpath('./data/demo/');      demo_v1;
addpath('./data/man/');      man_v1;
%addpath('./data/man/');      man_v2;
%addpath('./data/frog/');     frog_v1;
%addpath('./data/frog/');     frog_v2;
%addpath('./data/chiquita/'); chiquita_v1;
%addpath('./data/chiquita/'); chiquita_v2;
%addpath('./data/schooner/'); schooner_v1;
%addpath('./data/urn/');      urn_v1;
%addpath('./data/yong/');     yong_v1;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 2: Video Processing.

% Display progress.
disp('Performing video processing...');

% Modify extensions for low-resolution imagery (if enabled).
if useLowRes
   seqName = [seqName,'-lr'];
   calName = [calName,'-lr'];
end

% Get two points on the "middle" line (from user input).
disp('   + finding the dividing line between reference planes...');
disp('     (click on two points on the dividing line)');
frame = imread(['./data/',objName,'/',seqName,'/000001.jpg']);
figure(1); clf; set(gcf,'Name','Dividing Line Selection');
imagesc(frame); axis image;
title('Dividing Line Selection');
xlabel('click on two points on the dividing line');
middlePoints = ginput(2);

% Get bounding box for estimated "vertical" shadow boundary.
disp('   + define the reference area for the "vertical" plane...');
disp('     (click on top-left and bottom-right corners of reference area)');
figure(1); clf; set(gcf,'Name','Reference Area for "Vertical" Plane');
imagesc(frame); axis image;
title('Reference Area for "Vertical" Plane');
xlabel('click on the top-left and bottom-right corners of reference area');
x = ginput(2);
vRows = round(min(x(:,2))):round(max(x(:,2)));
vCols = round(min(x(:,1))):round(max(x(:,1)));

% Get bounding box for estimated "horizontal" shadow boundary.
disp('   + define the reference area for the "horizontal" plane...');
disp('     (click on top-left and bottom-right corners of reference area)');
figure(1); clf; set(gcf,'Name','Reference Area for "Horizontal" Plane');
imagesc(frame); axis image;
title('Reference Area for "Horizontal" Plane');
title('Reference Area for "Horizontal" Plane');
xlabel('click on the top-left and bottom-right corners of reference area');
x = ginput(2);
hRows = round(min(x(:,2))):round(max(x(:,2)));
hCols = round(min(x(:,1))):round(max(x(:,1)));

% Perform necessary video processing.
videoProcessing;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 3: Extrinsic Calibration.

% Display progress.
disp('Determining extrinsic calibration of reference plane(s)...');

% Load intrinsic calibration.
% Note: This assumes that you have already completed intrinsic calibration
%       for this sequence. See the assignment handout for more details.
load(['./data/',calName,'/Calib_Results'],...
   'fc','cc','kc','alpha_c','nx','ny');

% Obtain the extrinsic calibration for the "horizontal" plane.
disp('   + finding the extrinsic parameters of the "horizontal" plane....');
firstFrame = ['./data/',objName,'/',seqName,'/000001'];
[Tc_h,Rc_h,H_h] = ...
   computeExtrinsic(fc,cc,kc,alpha_c,firstFrame,'jpg',nX,nY,dX,dY);
pause(1.5);

% Obtain the extrinsic calibration for the "vertical" plane.
disp('   + finding the extrinsic parameters of the "vertical" plane....');
firstFrame = ['./data/',objName,'/',seqName,'/000001'];
[Tc_v,Rc_v,H_v] = ...
   computeExtrinsic(fc,cc,kc,alpha_c,firstFrame,'jpg',nX,nY,dX,dY);
pause(1.5);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 4: Reconstruction

% Display progress.
disp('Reconstructing 3D points using intersection with shadow plane(s)...');

% Estimate parameters of reference planes (using least-squares).
X = [0 dX dX 0; 0 0 dY dY; 0 0 0 0];
hPlane = fitPlane(X(1,:),X(2,:),X(3,:))';
X = Rc_h'*(Rc_v*X + repmat(Tc_v-Tc_h,1,size(X,2)));
vPlane = fitPlane(X(1,:),X(2,:),X(3,:))';

% Calculate camera center (in "horizontal" reference coordinate system).
C = -Rc_h'*Tc_h;

% Display progress.
disp('   + recovering implicit representation of shadow planes...');

% Determine implicit representation for the shadow planes.
% Note: There are several approaches for this step. See the IJCV paper
%       for a complete description, or consult the assignment handout.
shadowPlaneEnter = zeros(length(recFrames),4); % "entering" shadow plane
shadowPlaneLeave = zeros(length(recFrames),4); % "leaving" shadow plane
for i = 1:length(recFrames)

   % Determine true position of the "vertical" shadow boundary (entering).
   n1_v = Rc_h'*pixel2ray(intersectLines(vLineEnter(i,:),middleLine),fc,cc,kc,alpha_c);
   n2_v = Rc_h'*pixel2ray(intersectLines(vLineEnter(i,:),upperLine),fc,cc,kc,alpha_c);
   p1_v = intersectLineWithPlane(C,n1_v,vPlane);
   p2_v = intersectLineWithPlane(C,n2_v,vPlane);
   
   % Determine true position of the "horizontal" shadow boundary (entering).
   n1_h = Rc_h'*pixel2ray(intersectLines(hLineEnter(i,:),middleLine),fc,cc,kc,alpha_c);
   n2_h = Rc_h'*pixel2ray(intersectLines(hLineEnter(i,:),lowerLine),fc,cc,kc,alpha_c);
   p1_h = intersectLineWithPlane(C,n1_h,hPlane);
   p2_h = intersectLineWithPlane(C,n2_h,hPlane);
   
   % Compute the "entering" shadow plane parameters.
   q_v = p1_v;
   v_v = (p2_v-p1_v)/norm(p2_v-p1_v);
   q_h = p1_h;
   v_h = (p2_h-p1_h)/norm(p2_h-p1_h);
   shadowPlaneEnter(i,1:3) = cross(v_v,v_h);
   shadowPlaneEnter(i,1:3) = shadowPlaneEnter(i,1:3)/norm(shadowPlaneEnter(i,1:3));
   shadowPlaneEnter(i,4) = 0.5*shadowPlaneEnter(i,1:3)*(q_v+q_h);
   
   % Determine true position of the "vertical" shadow boundary (leaving).
   n1_v = Rc_h'*pixel2ray(intersectLines(vLineLeave(i,:),middleLine),fc,cc,kc,alpha_c);
   n2_v = Rc_h'*pixel2ray(intersectLines(vLineLeave(i,:),upperLine),fc,cc,kc,alpha_c);
   p1_v = intersectLineWithPlane(C,n1_v,vPlane);
   p2_v = intersectLineWithPlane(C,n2_v,vPlane);
   
   % Determine true position of the "horizontal" shadow boundary (leaving).
   n1_h = Rc_h'*pixel2ray(intersectLines(hLineLeave(i,:),middleLine),fc,cc,kc,alpha_c);
   n2_h = Rc_h'*pixel2ray(intersectLines(hLineLeave(i,:),lowerLine),fc,cc,kc,alpha_c);
   p1_h = intersectLineWithPlane(C,n1_h,hPlane);
   p2_h = intersectLineWithPlane(C,n2_h,hPlane);
   
   % Compute the "entering" shadow plane parameters.
   q_v = p1_v;
   v_v = (p2_v-p1_v)/norm(p2_v-p1_v);
   q_h = p1_h;
   v_h = (p2_h-p1_h)/norm(p2_h-p1_h);
   shadowPlaneLeave(i,1:3) = cross(v_v,v_h);
   shadowPlaneLeave(i,1:3) = shadowPlaneLeave(i,1:3)/norm(shadowPlaneLeave(i,1:3));
   shadowPlaneLeave(i,4) = 0.5*shadowPlaneLeave(i,1:3)*(q_v+q_h);
   
end

% Display progress.
disp('   + reconstructing 3D points...');

% Load first frame for assigning color.
% Note: You could use the maximum value image or some other source here.
frame = im2double(imread(['./data/',objName,'/',seqName,'/',...
   num2str(allFrames(1),'%0.6d'),'.jpg']));

% Reconstruct 3D points using intersection with shadow plane(s).
% Note: If multiple shadow planes are used, then reduce errors
%       by averaging available estimates. This can also assist
%       in removing some outliers (if estimates do not agree).
idx       = find(~isnan(shadowEnter) & ~isnan(shadowLeave));
idx       = idx(1:dSample:length(idx));
[row,col] = ind2sub(size(shadowEnter),idx);
npts      = length(idx);
vertices  = zeros(npts,3);
colors    = 0.65*ones(npts,3);
h = waitbar(0,'Reconstructing 3D points...');
for i = 1:npts
   
   % Obtain the camera ray for this pixel.
   n = Rc_h'*pixel2ray([col(i); row(i)],fc,cc,kc,alpha_c);
   
   % Interpolate "entering" shadow plane parameters (using shadow time).
   if ~isnan(shadowEnter(idx(i)))
      t  = shadowEnter(idx(i));
      t1 = floor(t);
      t2 = t1+1;
      if t2 <= length(recFrames)
         alpha = (t-t1)/(t2-t1);
         wEnter = (1-alpha)*shadowPlaneEnter(t1,:)+alpha*shadowPlaneEnter(t2,:);
         pEnter = intersectLineWithPlane(C,n,wEnter');
         vertices(i,:) = pEnter;
      end
   end
   
   % Interpolate "leaving" shadow plane parameters (using shadow time).
   if ~isnan(shadowLeave(idx(i)))
      t  = shadowLeave(idx(i));
      t1 = floor(t);
      t2 = t1+1;
      if t2 <= length(recFrames)
         alpha = (t-t1)/(t2-t1);
         wLeave = (1-alpha)*shadowPlaneLeave(t1,:)+alpha*shadowPlaneLeave(t2,:);
         pLeave = intersectLineWithPlane(C,n,wLeave');
         vertices(i,:) = pLeave;
      end
   end

   % Average "entering" and "leaving" estimates (if both are available).
   % Note: If points do not agree, set coordinate to infinity.
   %       This will ensure that it is clipped by the bounding volume.
   if ~isnan(shadowEnter(idx(i))) && ~isnan(shadowLeave(idx(i)))
      if norm(pEnter-pLeave) <= distReject
         vertices(i,:) = 0.5*(pEnter + pLeave);
      else
         vertices(i,:) = Inf*[1 1 1];
      end
   end
   
   % Assign color per vertex (using source image).
   colors(i,:) = frame(row(i),col(i),:);
   
   % Update progress bar.
   waitbar(i/npts);
      
end
close(h);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 5: Post-processing and Visualization

% Display progress.
disp('Display reconstruction results and exporting VRML file...');

% Display the 3D configuraton using the extrinsic calibration.
% Note: Some of this is hard-coded for the configuration included
%       with the assignment. This could be generalized.
disp('   + displaying reconstruction results...');
figure(1); set(gcf,'Name','Reconstruction Results'); clf;
X = [0 dX dX 0; 0 0 dY dY; 0 0 0 0];
patch(X(1,:),X(2,:),X(3,:),0.65*[1 1 1],'FaceAlpha',0.5);
hold on;
   % plots the "horizontal" plane
   plot3(X(1,:),X(2,:),X(3,:),'g.','MarkerSize',15); 
hold off;
X = Rc_h'*(Rc_v*X + repmat(Tc_v-Tc_h,1,size(X,2)));
patch(X(1,:),X(2,:),X(3,:),0.65*[1 1 1],'FaceAlpha',0.5);
hold on;
   % plots the "vertical" plane
   plot3(X(1,:),X(2,:),X(3,:),'r.','MarkerSize',15);
hold off;
C = -Rc_h'*Tc_h;
hold on;
   % plots the camera center
   plot3(C(1),C(2),C(3),'b.','MarkerSize',20);
hold off;
axis equal tight; grid on; view(3); box on;
axis([-200 800 -1300 500 -100 1000]);
xlabel('x'); ylabel('y'); zlabel('z');

% Clip the recovered 3D point cloud using the bounding volume.
clip = find( (vertices(:,1) >= clipRangeX(1) & vertices(:,1) <= clipRangeX(2)) & ...
             (vertices(:,2) >= clipRangeY(1) & vertices(:,2) <= clipRangeY(2)) & ...
             (vertices(:,3) >= clipRangeZ(1) & vertices(:,3) <= clipRangeZ(2)) );

% Display the recovered 3D point cloud (with per-vertex color).
% Note: Convert to indexed color map for use with FSCATTER3.
figure(1); set(gcf,'Name','Reconstruction Results');
C = reshape(colors,[size(colors,1) 1 size(colors,2)]);
[C,cmap] = rgb2ind(C,256);
hold on;
   %plot3(vertices(:,1),vertices(:,2),vertices(:,3),'b.','MarkerSize',5);
   fscatter3(vertices(:,1),vertices(:,2),vertices(:,3),double(C),cmap);
hold off;

% Export colored point cloud as a VRML file.
% Note: Interchange x and y coordinates for j3DPGP.
disp('   + exporting VRML file...');
writeVRML(['./models/',objName,'_',seqName,'.wrl'],...
   vertices(clip,[2 1 3]),...
   colors(clip,:));
disp(' ');
