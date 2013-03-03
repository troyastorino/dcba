
function points3D_cameraFrame = obtain3Dpoints_from_knownPlane(Camera_intrinsics,Camera_extrinsics, PlaneRotation_CameraFrame, PlaneTranslation_cameraFrame, points2D_CameraFrame)

% coordinate sistem of the screen plane
ScreenPlane_Normal_vect = PlaneRotation_CameraFrame(:,3); %is the Z coordinate of the rotation matrix
ScreenPlane_Origin = PlaneTranslation_cameraFrame;

% Normalize(augmented) the 2d points
ProjectedGrid_2dpoints_cameraPlane = cat(1,points2D_CameraFrame, ...
                                           ones(1,size(points2D_CameraFrame,2))...
                                         );
% obtain the rays corresponding to the corners
A_camera = [Camera_intrinsics [0 0 0]']* Camera_extrinsics; % Camrea transformation Kin*wKc
ProjectedGrid_3drays_cameraFrame = pinv(A_camera)*ProjectedGrid_2dpoints_cameraPlane;

% obtain the scale for the ray that corses the screen
ScreenZDistance =(-1)*dot(ScreenPlane_Origin, ScreenPlane_Normal_vect); %Obtain the Z distance, Projecting the Screen origin over its Z coordinate
RayZprojection = dot( ScreenPlane_Normal_vect(:)* ones(1,size( ProjectedGrid_3drays_cameraFrame, 2)), ...
                      ProjectedGrid_3drays_cameraFrame(1:3,:));
RayScale = (-1*ScreenZDistance) ./ RayZprojection;

% obtain the 3D points of the projected Grid
points3D_cameraFrame = (ones(3,1)*RayScale) .* ProjectedGrid_3drays_cameraFrame(1:3,:);

end