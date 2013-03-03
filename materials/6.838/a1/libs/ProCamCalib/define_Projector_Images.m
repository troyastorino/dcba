 if ~exist('n_ima')|~exist('KK')|~exist('fc')|~exist('cc')|~exist('kc')|~exist('alpha_c')%|~exist('fc_proj')|~exist('fc_cam'),
    fprintf(1,'No system calibration data available.\n');
    fprintf(1,'Load the camera calibration (Calibrate the camera and save it; if needed).\n');
    return;
 end;
 
% select the source for the projector planes (the images used to calibrate
% the camera or other source)

quest_sameImages = input('Do you want use the same images used for camera calibration to calibrate the projector? ([]=yes, other=no) ');
quest_sameImages = ~isempty(quest_sameImages);

if ~quest_sameImages
    %-- Save the camera calibration results in side variables
    %   and the rotation and translation of each plane
    save Camera_data KK fc cc kc alpha_c dX dY wintx winty n_ima calib_name first_num format_image nx ny Origin_active_images
    for i=ind_active
        eval(['save Camera_data Tc_' num2str(i) ' Rc_' num2str(i) ' -append;']);
    end

    clear all
    load Camera_data

else
    %-- Save the camera calibration results
    save Camera_data KK fc cc kc alpha_c
    clear all;
    msgbox('Select the projector calibration images and select the printed pattern');
    
    add_suppress
    dont_ask = 1; % select the printed pattern in all the images
    click_calib;
    %----------------
    load Camera_data fc cc kc alpha_c
    for i=ind_active
    %go_calib_optim;
        eval(['[omc_' num2str(i) ',Tc_' num2str(i) ',Rc_' num2str(i) ',H_' num2str(i) '] = compute_extrinsic(x_' num2str(i) ',X_' num2str(i) ',fc,cc,kc,alpha_c);']);

    end
    %------------------
    Origin_active_images = ind_active;
    save Camera_data dX dY wintx winty n_ima calib_name first_num format_image nx ny Origin_active_images -append
    for i=ind_active
        eval(['save Camera_data Tc_' num2str(i) ' Rc_' num2str(i) ' -append;']);
    end

    clear all
    load Camera_data
end
Projector_images_check = [];