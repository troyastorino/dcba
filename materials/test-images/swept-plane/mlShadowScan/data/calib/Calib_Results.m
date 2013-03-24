% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 2063.624094669945900 ; 2068.246294767563500 ];

%-- Principal point:
cc = [ 564.284599933070470 ; 430.077119141454150 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.189857971274649 ; 1.159545847481371 ; 0.006814823434558 ; 0.003184022300329 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 30.374575543046941 ; 33.199481918617479 ];

%-- Principal point uncertainty:
cc_error = [ 45.139218736073282 ; 39.839110752045663 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.077785310936755 ; 1.175019056787757 ; 0.004834221843810 ; 0.005156243821439 ; 0.000000000000000 ];

%-- Image size:
nx = 1024;
ny = 768;


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
omc_1 = [ 2.045219e+000 ; 6.973649e-003 ; -9.920681e-003 ];
Tc_1  = [ -1.406886e+002 ; 1.649547e+002 ; 1.608946e+003 ];
omc_error_1 = [ 1.892898e-002 ; 1.280966e-002 ; 2.277074e-002 ];
Tc_error_1  = [ 3.528540e+001 ; 3.125616e+001 ; 2.618866e+001 ];

%-- Image #2:
omc_2 = [ 2.739892e+000 ; 2.204545e-002 ; -8.118468e-002 ];
Tc_2  = [ -1.266679e+002 ; 4.966804e+001 ; 1.632744e+003 ];
omc_error_2 = [ 2.180154e-002 ; 6.258541e-003 ; 3.262036e-002 ];
Tc_error_2  = [ 3.571913e+001 ; 3.152480e+001 ; 2.485260e+001 ];

%-- Image #3:
omc_3 = [ -2.664957e+000 ; 2.309186e-002 ; -3.838045e-002 ];
Tc_3  = [ -1.482197e+002 ; -8.976867e+001 ; 1.793539e+003 ];
omc_error_3 = [ 2.166759e-002 ; 6.138080e-003 ; 2.489550e-002 ];
Tc_error_3  = [ 3.932237e+001 ; 3.465596e+001 ; 2.682987e+001 ];

%-- Image #4:
omc_4 = [ -2.572981e+000 ; -2.737203e-001 ; 9.751961e-001 ];
Tc_4  = [ 8.120777e+001 ; -5.481493e+001 ; 1.725026e+003 ];
omc_error_4 = [ 1.894030e-002 ; 9.069222e-003 ; 2.910351e-002 ];
Tc_error_4  = [ 3.786914e+001 ; 3.322386e+001 ; 2.676943e+001 ];

%-- Image #5:
omc_5 = [ -2.844868e+000 ; -5.685572e-001 ; 1.135201e+000 ];
Tc_5  = [ 1.698489e+001 ; -1.960257e+001 ; 1.691517e+003 ];
omc_error_5 = [ 2.061930e-002 ; 7.124572e-003 ; 3.356766e-002 ];
Tc_error_5  = [ 3.700356e+001 ; 3.254619e+001 ; 2.602475e+001 ];

%-- Image #6:
omc_6 = [ -2.585083e+000 ; 3.775269e-001 ; -9.700732e-001 ];
Tc_6  = [ -2.907580e+002 ; 2.529153e+001 ; 1.582694e+003 ];
omc_error_6 = [ 2.070426e-002 ; 7.480621e-003 ; 2.794150e-002 ];
Tc_error_6  = [ 3.496754e+001 ; 3.087289e+001 ; 2.754000e+001 ];

%-- Image #7:
omc_7 = [ 2.891098e+000 ; -5.059458e-001 ; 8.380683e-001 ];
Tc_7  = [ -2.776889e+002 ; 6.394625e+001 ; 1.559041e+003 ];
omc_error_7 = [ 1.690869e-002 ; 9.474044e-003 ; 3.366460e-002 ];
Tc_error_7  = [ 3.421586e+001 ; 3.044754e+001 ; 2.725572e+001 ];

%-- Image #8:
omc_8 = [ 2.424278e+000 ; -4.245380e-001 ; 1.070571e+000 ];
Tc_8  = [ -1.400601e+002 ; 5.341099e+001 ; 1.456319e+003 ];
omc_error_8 = [ 1.841168e-002 ; 1.430155e-002 ; 2.804698e-002 ];
Tc_error_8  = [ 3.181727e+001 ; 2.821281e+001 ; 2.424571e+001 ];

%-- Image #9:
omc_9 = [ -2.440356e+000 ; 6.902214e-001 ; 9.714626e-001 ];
Tc_9  = [ 8.722573e+001 ; -6.660062e+001 ; 1.685489e+003 ];
omc_error_9 = [ 1.726551e-002 ; 1.427568e-002 ; 2.640136e-002 ];
Tc_error_9  = [ 3.697351e+001 ; 3.250578e+001 ; 2.520881e+001 ];

%-- Image #10:
omc_10 = [ 2.358254e+000 ; -4.865555e-001 ; 2.230819e-001 ];
Tc_10  = [ -7.296532e+001 ; 1.659820e+002 ; 1.522842e+003 ];
omc_error_10 = [ 1.885433e-002 ; 1.100591e-002 ; 2.628068e-002 ];
Tc_error_10  = [ 3.337770e+001 ; 2.953831e+001 ; 2.452922e+001 ];

