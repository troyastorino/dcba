
% Specify the object and reconstruction parameters.
objName   = 'frog';        % object name (should correspond to a data dir.)
calName   = 'calib';       % calibration source (also must be a data dir.)
seqName   = 'v2';          % sequence name (subdirectory of object)
allFrames = 1:200;         % all frames in this sequence
recFrames = 74:174;        % frames used for reconstruction

% Set reconstruction parameters.
useLowRes   = true;         % enables low resolution imagery (for debugging)
minContrast = 50;           % pixel exclusion threshold (min. contrast)
dSample     = 1;            % down-sampling rate for final point cloud
distReject  = 20;           % rejection distance (distance in mm for outlier removal)

% Define "horizonal" and "vertical" reference plane properties.
% Note: These lengths are used to determine the extrinsic calibration from 
%       the distances between printed fiducials on each foamcore plane.
nX = 1;        % number of rectangles along x-axis
nY = 1;        % number of rectangles along y-axis
dX = 558.8;    % length along x-axis (mm) (between checkerboard rectangles)
dY = 303.2125; % length along y-axis (mm) (between checkerboard rectangles)

% Define bounding volume to clip final results.
% Note: This eliminates outliers and crops foamcore boards from 3D model.
clipRangeX = [5 dX-5];  % clip range along x-axis (mm)
clipRangeY = [5 dY-5];  % clip range along y-axis (mm)
clipRangeZ = [5 dY+50]; % clip range along z-axis (mm)