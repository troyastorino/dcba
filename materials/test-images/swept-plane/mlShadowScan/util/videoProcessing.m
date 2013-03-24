
% VIDEOPROCESSING Implements video processing functions.
%    VIDEOPROCESSING implements the necessary video processung functions
%    required by SHADOWSCAN. As described in the assignment handout,
%    this function accomplished two key tasks: (1) determination of
%    fraction shadow-crossing times and (2) least-squares estimation 
%    of the shadow boundaries. For more information see the accompanying
%    assignment handout.
%
% Douglas Lanman and Gabriel Taubin 
% Brown University
% 18 May 2009


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 1: Determine the dynamic range and shadow threshold for each pixel.

% Display progress.
disp('   + estimating per-pixel dynamic range and shadow thresholds...');

% Extract the image dimensions.
frameDim = [size(frame,1) size(frame,2)];

% Determine the minimum and maximum values observed in each pixel.
maxValue = -Inf*ones(frameDim);
minValue =  Inf*ones(frameDim);
figure(1); clf; set(gcf,'Name','Minimum Value Per-pixel');
h = waitbar(0,'Estimating per-pixel dynamic range...');
for i = 1:length(allFrames)
   frame = imread(['./data/',objName,'/',seqName,'/',num2str(allFrames(i),'%0.6d'),'.jpg']);
   frame = rgb2gray(frame);
   minValue(frame < minValue) = frame(frame < minValue);
   maxValue(frame > maxValue) = frame(frame > maxValue);
   imagesc(minValue,[0 255]); axis image; colormap gray;
   title('Minimum Value Per-pixel'); drawnow;
   waitbar(i/length(allFrames));
end
close(h);

% Assign the shadow threshold for each pixel.
shadowValue = 0.5*(minValue + maxValue);
figure(1); clf; set(gcf,'Name','Shadow Threshold Per-pixel');
imagesc(shadowValue,[0 255]); axis image; colormap gray;
title('Shadow Threshold Per-pixel'); pause(3.0);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 2: Estimate the parameters of the shadow plane(s).

% Display progress.
disp('   + estimating shadow boundaries...');

% Calculate equations for lower/middle/upper boundaries.
% Note: These boundaries are useful for clipping the estimated lines.
lowerLine  = fitLine([0 frameDim(2)]+0.5,frameDim(1)*[1 1]+0.5);
middleLine = fitLine(middlePoints(:,1),middlePoints(:,2));
upperLine  = fitLine([0 frameDim(2)]+0.5,0.5*[1 1]);

% Estimate the shadow plane(s) for each frame.
vLineEnter   = zeros(length(recFrames),3); % vertical "entering" shadow
vLineLeave   = zeros(length(recFrames),3); % vertical "leaving" shadow
hLineEnter   = zeros(length(recFrames),3); % horizontal "entering" shadow
hLineLeave   = zeros(length(recFrames),3); % horizontal "leaving" shadow
vRowPosEnter = zeros(size(vRows)); % column of vertical "entering" shadow
vRowPosLeave = zeros(size(vRows)); % column of vertical "leaving" shadow
hRowPosEnter = zeros(size(hRows)); % column of horizontal "entering" shadow
hRowPosLeave = zeros(size(hRows)); % column of horizontal "leaving" shadow
figure(1); clf;  set(gcf,'Name','Shadow Boundaries'); 
h = waitbar(0,'Estimating the shadow boundaries...');
for i = 1:length(recFrames)
   
   % Extract the current frame (and convert to double-precision grayscale).
   frame = imread(['./data/',objName,'/',seqName,'/',num2str(recFrames(i),'%0.6d'),'.jpg']);
   frame = double(rgb2gray(frame));
   
   % Evaluate the "vertical" shadow line.
   vImg = frame(vRows,vCols)-shadowValue(vRows,vCols);
   for j = 2:length(vCols)
      idx = (vImg(:,j) >= 0) & (vImg(:,j-1) < 0);
      vRowPosEnter(idx) = (j-1) + (-vImg(idx,j-1))./(vImg(idx,j)-vImg(idx,j-1))+vCols(1)-1;
      idx = (vImg(:,j) < 0) & (vImg(:,j-1) >= 0);
      vRowPosLeave(idx) = (j-1) + (-vImg(idx,j-1))./(vImg(idx,j)-vImg(idx,j-1))+vCols(1)-1;
   end
   vLineEnter(i,:) = fitLine(vRowPosEnter,vRows);
   vLineLeave(i,:) = fitLine(vRowPosLeave,vRows);
   
   % Evaluate the "horizontal" shadow line.
   hImg = frame(hRows,hCols)-shadowValue(hRows,hCols);
   for j = 2:length(hCols)
      idx = (hImg(:,j) >= 0) & (hImg(:,j-1) < 0);
      hRowPosEnter(idx) = (j-1) + (-hImg(idx,j-1))./(hImg(idx,j)-hImg(idx,j-1))+hCols(1)-1;
      idx = (hImg(:,j) < 0) & (hImg(:,j-1) >= 0);
      hRowPosLeave(idx) = (j-1) + (-hImg(idx,j-1))./(hImg(idx,j)-hImg(idx,j-1))+hCols(1)-1;
   end
   hLineEnter(i,:) = fitLine(hRowPosEnter,hRows);
   hLineLeave(i,:) = fitLine(hRowPosLeave,hRows);
   
   % Display the shadow lines.
   imagesc(frame,[0 255]); axis image; colormap gray;
   title('Shadow Boundaries'); drawnow;
   hold on;
      plot(vRowPosEnter,vRows,'b.');
      plot(vRowPosLeave,vRows,'g.');
      plot(hRowPosEnter,hRows,'b.');
      plot(hRowPosLeave,hRows,'g.');
   hold off;
   hold on;
      p1 = intersectLines(vLineEnter(i,:),middleLine);
      p2 = intersectLines(vLineEnter(i,:),upperLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'b');
      p1 = intersectLines(vLineLeave(i,:),middleLine);
      p2 = intersectLines(vLineLeave(i,:),upperLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'g');
      p1 = intersectLines(hLineEnter(i,:),lowerLine);
      p2 = intersectLines(hLineEnter(i,:),middleLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'b');
      p1 = intersectLines(hLineLeave(i,:),lowerLine);
      p2 = intersectLines(hLineLeave(i,:),middleLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'g');
   hold off;
   waitbar(i/length(recFrames));
end
close(h);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 3: Estimate the per-pixel shadow crossing time(s).

% Display progress.
disp('   + estimating shadow crossing time(s)...');

% Determine the shadow crossing times for each pixel.
% Note: Will store first entry and last exit of the shadow at each pixel.
shadowEnter = NaN*ones(size(shadowValue)); % time shadow "enters" pixel
shadowLeave = NaN*ones(size(shadowValue)); % time shadow "leaves" pixel
frame2 = imread(['./data/',objName,'/',seqName,'/',num2str(recFrames(1),'%0.6d'),'.jpg']);
frame2 = double(rgb2gray(frame2));
h = waitbar(0,'Estimating shadow crossing time(s)...');
for i = 2:length(recFrames)
   frame1 = frame2;
   frame2 = imread(['./data/',objName,'/',seqName,'/',num2str(recFrames(i),'%0.6d'),'.jpg']);
   frame2 = double(rgb2gray(frame2));
   idx = (frame1 >= shadowValue) & (frame2 <  shadowValue) & isnan(shadowEnter);
   shadowEnter(idx) = ...
      (i-1) + (shadowValue(idx)-frame1(idx))./(frame2(idx)-frame1(idx));
   idx = (frame1 <  shadowValue) & (frame2 >= shadowValue);
   shadowLeave(idx) = ...
      (i-1) + (shadowValue(idx)-frame1(idx))./(frame2(idx)-frame1(idx));
   waitbar(i/length(recFrames));
end
close(h);
shadowEnter((maxValue - minValue) < minContrast) = NaN; % min. contrast test
shadowLeave((maxValue - minValue) < minContrast) = NaN; % min. contrast test
figure(1); set(gcf,'Name','Shadow Time (First Crossing)'); clf;
imagesc(shadowEnter); axis image; colormap(jet(2^8)); %colorbar;
title('Shadow Time (First Crossing)'); pause(3.0);
figure(1); set(gcf,'Name','Shadow Time (Second Crossing)'); clf;
imagesc(shadowLeave); axis image; colormap(jet(2^8)); %colorbar;
title('Shadow Time (Second Crossing)'); pause(3.0);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part 4: Visualize video processing results (i.e., shadow detection).

% Display progress.
disp('   + displaying video processing results...');

% Display the estimated shadow times (rounded to nearest frame).
dt = 0.25;
figure(1); clf; set(gcf,'Name','Video Processing Results');
for i = 1:5:length(recFrames)
   frame = imread(['./data/',objName,'/',seqName,'/',num2str(recFrames(i),'%0.6d'),'.jpg']);
   imagesc(frame,[0 255]); axis image; colormap gray; 
   title('Video Processing Results'); drawnow;
   hold on;
      [rows,cols] = find((shadowEnter >= (i-dt)) & (shadowEnter < (i+dt)) & ...
                         (shadowEnter < shadowLeave) & ( abs(maxValue-minValue) > minContrast) );
      plot(cols,rows,'b.');
      [rows,cols] = find((shadowLeave >= (i-dt)) & (shadowLeave < (i+dt)) & ...
                         (shadowEnter < shadowLeave) & ( abs(maxValue-minValue) > minContrast) );
      plot(cols,rows,'g.'); 
   hold off;
   hold on;
      p1 = intersectLines(vLineEnter(i,:),middleLine);
      p2 = intersectLines(vLineEnter(i,:),upperLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'b');
      p1 = intersectLines(vLineLeave(i,:),middleLine);
      p2 = intersectLines(vLineLeave(i,:),upperLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'g');
      p1 = intersectLines(hLineEnter(i,:),lowerLine);
      p2 = intersectLines(hLineEnter(i,:),middleLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'b');
      p1 = intersectLines(hLineLeave(i,:),lowerLine);
      p2 = intersectLines(hLineLeave(i,:),middleLine);
      plot([p1(1) p2(1)],[p1(2) p2(2)],'g');
   hold off;
   drawnow;
end
