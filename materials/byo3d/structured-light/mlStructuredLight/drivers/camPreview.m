function camPreview(camera)

% CAMPREVIEW Previews camera capture.
%    CAMPREVIEW captures frame from any camera supported 
%    by the Matlab Image Acquisition Toolbox. The image 
%    stream is displayed in a Matlab figure window until
%    the user presses the stop button.
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

% Create video capture interface.
figure(1); clf; 
set(gcf,'Name','Camera Preview');
set(gcf,'MenuBar','none');
I = camCapture(camera);
set(gca,'Pos',[0 0 1 1]);
hImage = imagesc(I{1});
axis image off;

% Create stop pushbutton.
uiButton = uicontrol('Style','pushbutton',...
   'Units', 'normalized',...
   'Position',[0.015 0.0175 0.1 0.05],...
   'Value',1,'String','Stop');

% Preview video sequence.
while get(uiButton,'value') 
   I = camCapture(camera);
   set(hImage,'Cdata',I{1});
end
close(figure(1));
