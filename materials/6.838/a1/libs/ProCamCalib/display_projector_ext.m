%% Shows the extrinsic parameters:
dX=15;
dY=15;
for i=ind_active
    % seting the number of squares for each pattern. Both X and Y
    eval(['n_sq_x_' num2str(i) '= ' num2str(n_sq_x) '; n_sq_y_' num2str(i) '=' num2str(n_sq_y) ';']);
end
%-- Projector parameters:
%%
fc_proj  = fc;
cc_proj = cc;
kc_proj = kc;
alpha_c_proj = alpha_c;
fc_error_proj  = fc_error;
cc_error_proj = cc_error;
kc_error_proj = kc_error;
alpha_c_error_proj = alpha_c_error;

est_fc_proj = est_fc;
est_dist_proj = est_dist;
est_alpha_proj = est_alpha;
center_optim_proj = center_optim;
nx_proj = nx;
ny_proj = ny;
active_images_proj = active_images;
ind_active_proj = ind_active;

% Position of the global structure wrt the projector:
eval(['T_proj = Tc_' num2str(ind_active(1)) ';' ]);
om_proj = omc_1;
R_proj = rodrigues(om_proj);
T_error_proj = Tc_error_1;
om_error_proj = omc_error_1;

% Loading camera parameters
load Calib_Results alpha_c fc cc kc nx ny
alpha_c_cam = alpha_c;
fc_cam = fc;
cc_cam = cc;
kc_cam =kc;
nx_cam = nx;
ny_cam = ny;


% Restore Projector parameters

fc = fc_proj;
cc = cc_proj;
kc = kc_proj;
alpha_c = alpha_c_proj;
nx = nx_proj;
ny = ny_proj;






%%
clear dX dY
ext_calib_cam_proj;