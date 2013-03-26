
% Reset Matlab environment.
clear; clc;

% Add required subdirectories.
addpath('../../utilities');
addpath('../../pgr/matSrc/');

% Set calibration parameters.
calibCam    = false; % enable/disable camera calibration data capture
calibProj   = false; % enable/disable projector calibration data capture
calibColor  = true;  % enable/disable color calibration data capture
projValue   = 255;   % Gray code intensity
pgrShutter  = 400;   % shutter speed for PGR camera(s)
screenIndex = 2;     % index of projector display (1 = first, 2 = second).
nImages     = 30;    % number of calibration images to capture.

% Get projector display properties.
% Note: Assumes the Matlab Psychotoolbox is installed.
window = screen('OpenWindow',screenIndex,projValue*[1 1 1]); 
rect   = screen('Rect',window); clc;
height = rect(4); width  = rect(3);
screen('MATLABToFront');

% Initialize PGR camera(s).
% Note: Make sure to optimize the PGR camera settings.
context = pgrInit;
for i = 1:length(context)
   matSetFlyShutterSpeed(context(i),pgrShutter,pgrShutter);
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part I: Camera Calibration

% Capture/save calibration images for PGR camera(s).
if calibCam
   rmdir('./cam','s'); mkdir('./cam');
   for i = 1:nImages

      % Prompt user to capture next image.
      disp(['Press any key to capture camera calibration image ',int2str(i),'...']);
      pause;

      % Capture image(s) from PGR camera(s).
      I = pgrCapture(context);

      % Save calibration image(s).
      for j = 1:length(context)
         if ~exist(['./cam/v',int2str(j)],'file')
            mkdir(['./cam/v',int2str(j)]);
         end
         imwrite(I{j},['./cam/v',int2str(j),'/',num2str(i,'%0.3d'),'.bmp']);
         imagesc(I{j}); axis image;
      end

   end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part II: Projector Calibration

% Create checkerboard image (for projector calibration).
% Note: Assumes 1024x768 projector resolution.
if calibProj
   %I = uint8(projValue*(checkerboard(64,6,8) > 0.5));
   I = projValue*ones(height,width,'uint8');
   C = uint8(projValue*(checkerboard(64,4,4) > 0.5));
   I((1:size(C,1))+(2*64)*1,(1:size(C,2))+(2*64)*2) = C;
   screen('PutImage',window,I);
   screen('Flip',window); pause(1.0);

   % Capture/save calibration images for projector calibration.
   rmdir('./proj','s'); mkdir('./proj');
   for i = 1:nImages

      % Prompt user to capture next image.
      disp(['Press any key to capture projector calibration image ',int2str(i),'...']);
      pause;

      % Capture image(s) from PGR camera(s).
      I = pgrCapture(context);

      % Save calibration image(s).
      for j = 1:length(context)
         if ~exist(['./proj/v',int2str(j)],'file')
            mkdir(['./proj/v',int2str(j)]);
         end
         imwrite(I{j},['./proj/v',int2str(j),'/',num2str(i,'%0.3d'),'.bmp']);
         imagesc(I{j}); axis image;
      end

   end
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part III: Color Calibration

% Run color calibration and record camera image(s).
if calibColor
   onImg  = uint8(projValue*ones(height,width));
   offImg = zeros(height,width,'uint8');
   clear C;
   C{1}   = Screen('MakeTexture',window,cat(3,onImg,offImg,offImg));
   C{2}   = Screen('MakeTexture',window,cat(3,offImg,onImg,offImg));
   C{3}   = Screen('MakeTexture',window,cat(3,offImg,offImg,onImg));
   for i = 1:3

      % Display/capture calibration image for this channel.
      Screen('CopyWindow',C{i},window,rect,rect);
      Screen('Flip',window);

      % Prompt user, then capture next image.
      disp(['Press any key to capture color calibration image ',int2str(i),'...']);
      pause;
      I = pgrCapture(context);
      for j = 1:length(context)
         if ~exist(['./cam/v',int2str(j)],'file')
            mkdir(['./cam/v',int2str(j)]);
         end
         imwrite(I{j},['./cam/v',int2str(j),'/c',num2str(i),'.bmp']);
         save(['./cam/v',int2str(j),'/projValue.mat'],'projValue');
         imagesc(I{j}); axis image;
      end

   end
end

% Close the fullscreen display and stop PGR camera(s).
screen('CloseAll');
pgrStop(context);
