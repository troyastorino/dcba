% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 1029.088963049301100 ; 1028.772845684173700 ];

%-- Principal point:
cc = [ 287.368565122283710 ; 204.931308136716550 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.148348422140938 ; 0.215129139753359 ; 0.004513111567607 ; 0.004877209469556 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 17.070043715418674 ; 18.263987634583518 ];

%-- Principal point uncertainty:
cc_error = [ 27.412500281800273 ; 22.806875607135709 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.085725043781932 ; 1.134230738991625 ; 0.005220394520242 ; 0.006113308879498 ; 0.000000000000000 ];

%-- Image size:
nx = 512;
ny = 384;


%-- Various other variables (may be ignored if you do not use the Matlab Calibration Toolbox):
%-- Those variables are used to control which intrinsic parameters should be optimized

n_ima = 10;						% Number of calibration images
est_fc = [ 1 ; 1 ];					% Estimation indicator of the two focal variables
est_aspect_ratio = 1;				% Estimation indicator of the aspect ratio fc(2)/fc(1)
center_optim = 1;					% Estimation indicator of the principal point
est_alpha = 0;						% Estimation indicator of the skew coefficient
est_dist = [ 1 ; 1 ; 1 ; 1 ; 0 ];	% Estimation indicator of the distortion coefficients


%-- Extrinsic parameters:
%-- The rotation (omc_kk) and the translation (Tc_kk) vectors for every calibration image and their uncertainties

%-- Image #1:
omc_1 = [ 2.036578e+000 ; 3.580816e-003 ; -5.042894e-003 ];
Tc_1  = [ -1.492369e+002 ; 1.808440e+002 ; 1.603248e+003 ];
omc_error_1 = [ 2.158304e-002 ; 1.557602e-002 ; 2.754627e-002 ];
Tc_error_1  = [ 4.284691e+001 ; 3.592680e+001 ; 2.924306e+001 ];

%-- Image #2:
omc_2 = [ 2.736359e+000 ; 2.030272e-002 ; -7.580010e-002 ];
Tc_2  = [ -1.353195e+002 ; 6.540022e+001 ; 1.627397e+003 ];
omc_error_2 = [ 2.430936e-002 ; 7.513096e-003 ; 3.845814e-002 ];
Tc_error_2  = [ 4.335610e+001 ; 3.620391e+001 ; 2.782033e+001 ];

%-- Image #3:
omc_3 = [ -2.677412e+000 ; 2.220340e-002 ; -4.491041e-002 ];
Tc_3  = [ -1.576714e+002 ; -7.275732e+001 ; 1.788436e+003 ];
omc_error_3 = [ 2.564636e-002 ; 7.294539e-003 ; 3.183379e-002 ];
Tc_error_3  = [ 4.769811e+001 ; 3.977096e+001 ; 3.016686e+001 ];

%-- Image #4:
omc_4 = [ -2.583017e+000 ; -2.717119e-001 ; 9.769155e-001 ];
Tc_4  = [ 7.201934e+001 ; -3.840784e+001 ; 1.718983e+003 ];
omc_error_4 = [ 2.288112e-002 ; 1.055353e-002 ; 3.498669e-002 ];
Tc_error_4  = [ 4.588860e+001 ; 3.808597e+001 ; 2.985905e+001 ];

%-- Image #5:
omc_5 = [ -2.853005e+000 ; -5.642614e-001 ; 1.137384e+000 ];
Tc_5  = [ 8.089040e+000 ; -3.506503e+000 ; 1.684811e+003 ];
omc_error_5 = [ 2.488755e-002 ; 8.304270e-003 ; 4.025044e-002 ];
Tc_error_5  = [ 4.488101e+001 ; 3.731199e+001 ; 2.896157e+001 ];

%-- Image #6:
omc_6 = [ -2.590345e+000 ; 3.739468e-001 ; -9.842379e-001 ];
Tc_6  = [ -2.985447e+002 ; 4.057904e+001 ; 1.573557e+003 ];
omc_error_6 = [ 2.390310e-002 ; 8.989839e-003 ; 3.384762e-002 ];
Tc_error_6  = [ 4.230018e+001 ; 3.534908e+001 ; 3.061237e+001 ];

%-- Image #7:
omc_7 = [ 2.881994e+000 ; -5.024460e-001 ; 8.501254e-001 ];
Tc_7  = [ -2.854324e+002 ; 7.902987e+001 ; 1.549958e+003 ];
omc_error_7 = [ 1.936219e-002 ; 1.092014e-002 ; 3.986221e-002 ];
Tc_error_7  = [ 4.143603e+001 ; 3.488539e+001 ; 3.034921e+001 ];

%-- Image #8:
omc_8 = [ 2.415125e+000 ; -4.205237e-001 ; 1.078310e+000 ];
Tc_8  = [ -1.476982e+002 ; 6.745370e+001 ; 1.450087e+003 ];
omc_error_8 = [ 2.150896e-002 ; 1.660169e-002 ; 3.381205e-002 ];
Tc_error_8  = [ 3.860677e+001 ; 3.236344e+001 ; 2.699559e+001 ];

%-- Image #9:
omc_9 = [ -2.451109e+000 ; 6.940105e-001 ; 9.658685e-001 ];
Tc_9  = [ 7.821627e+001 ; -5.066258e+001 ; 1.681070e+003 ];
omc_error_9 = [ 2.054012e-002 ; 1.656455e-002 ; 3.277058e-002 ];
Tc_error_9  = [ 4.484311e+001 ; 3.728243e+001 ; 2.825961e+001 ];

%-- Image #10:
omc_10 = [ 2.351035e+000 ; -4.874077e-001 ; 2.300602e-001 ];
Tc_10  = [ -8.103760e+001 ; 1.809039e+002 ; 1.516791e+003 ];
omc_error_10 = [ 2.140872e-002 ; 1.312883e-002 ; 3.168021e-002 ];
Tc_error_10  = [ 4.052031e+001 ; 3.392785e+001 ; 2.734238e+001 ];

