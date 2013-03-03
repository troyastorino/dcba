if ~exist('Projector_images_check')
   fprintf(1,'The projector image set needs initialization.\n');
   return;
end;

if ~exist('ind_active') |~exist('n_ima') |~exist('active_images') 
   dummyCode;
end

save 3DPoints ind_active n_ima active_images Origin_active_images

msgbox('Select the projected pattern');

disp('Window size for corner finder (wintx and winty):');
wintx = input('wintx ([] = 5) = ');
if isempty(wintx), wintx = 5; end;
wintx = round(wintx);
winty = input('winty ([] = 5) = ');
if isempty(winty), winty = 5; end;
winty = round(winty);

fprintf(1,'Window size = %dx%d\n',2*wintx+1,2*winty+1);

for i= ind_active

    %   extrinsic_computation;
    % Return:
    % Rc_ext rotation matrix of the reference from the camera to the plane.
    % Tc_ext translation from the camera frame to the grid frame in camera

    I = double(rgb2gray(imread( [calib_name num2str(first_num+i-1) '.' format_image])));

    display(['Processing Image number ' num2str(i)]);
    ProjectedGrid_2dpoints_cameraPlane = extract_grid(I,wintx,winty,fc,cc,kc,dX,dY);

    eval( ...
        ['X_' num2str(i) '= obtain3Dpoints_from_knownPlane(KK,eye(4), Rc_' num2str(i) ', Tc_' num2str(i) ', ProjectedGrid_2dpoints_cameraPlane);']...
        );
    eval(['save 3DPoints X_' num2str(i) ' -append;']);
end

clear all 
load 3DPoints

%% There may be a better way to ask for the projected image.
fprintf(1,'\n');
image_name = input('Projected image name: ','s');
I = double(rgb2gray(imread(image_name)));
[ProjectedGrid_2dpoints_projectorFrame,dumb,n_sq_x,n_sq_y] = extract_grid(I);%,wintx,winty,fc,cc,kc,dX,dY);


%%
Projector_3Dpoints_check=1;

display done;
