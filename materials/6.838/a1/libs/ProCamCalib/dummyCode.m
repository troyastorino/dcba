% im_labtop=imread('labtop.JPG');
% load Camera_data;
% 
% extract_grid(im_labtop(:,:,1),15,15,fc,cc,kc,dX,dY);
% % 
% load Plane
% TwoDpoint = [KK [0; 0; 0]]*[R_proj T_proj; 0 0 0 1]*[Tc_ext; 1];
% TwoDpointNorm=TwoDpoint/TwoDpoint(3)
% TestImage=zeros(size(I));
% TestImage(:,ceil(TwoDpointNorm(1)))=255;
% TestImage(ceil(TwoDpointNorm(2)),:)=255;
% %TestImage(ceil(TwoDpointNorm(2)),ceil(TwoDpointNorm(1)))=255;
% imshow(TestImage, 'DisplayRange', []);
% imwrite(TestImage,'test.png', 'png' );

if ~exist('active_images')
    active_images = Origin_active_images;
    ind_active = find(active_images);
end
active_images_cpy = active_images;     
add_suppress;                          
nonOrigin_active_images= find((Origin_active_images-active_images)==-1);  
if ~isempty(nonOrigin_active_images),   
fprintf ( 'Images [');          
for i=nonOrigin_active_images;     
fprintf ( ' %d',i); 
end; 
fprintf ( ' ] where not used for the initial camera calibration. \n '); 
fprintf ( 'In order to use it/them Set another Projector''s calib. images \n '); 
active_images = active_images_cpy;  
ind_active = find(active_images);
end; 