import unittest
import scipy.io
import os.path
import numpy as np
from numpy.testing import assert_array_less
from scan.data_capture.image import load_from_directory
from scan.data_capture.calibration import calibrate_with_checkerboard
from scan.util import rel_to_file

# get path to current directory


class TestCameraCalibration(unittest.TestCase):
    def setUp(self):
        # excess epsilon to add margin
        self.eps = np.nextafter(0, 1)
        
        # set up input data dir, parameters
        self.interior_corners = (8,6)
        self.side_length = 1.89

        # use results from matlab script to test
        self.calib_folder = rel_to_file("test-data/calib/calib", __file__)
        self.calib_images = load_from_directory(self.calib_folder)
        self.calib_results = scipy.io.loadmat(os.path.join(self.calib_folder, "Calib_Results.mat"), squeeze_me=True)

        # run the calibration
        self.cam = calibrate_with_checkerboard(None, self.calib_images, self.interior_corners, self.side_length)

    def test_focal_length(self):
        assert_array_less(np.abs(self.cam.focal_length - self.calib_results['fc']), self.calib_results['fc_error'] + self.eps)

    def test_principal_point(self):
        assert_array_less(np.abs(self.cam.principal_point - self.calib_results['cc']), self.calib_results['cc_error'] + self.eps)

    def test_alpha(self):
        assert_array_less(np.abs(self.cam.alpha - self.calib_results['alpha_c']), self.calib_results['alpha_c_error'] + self.eps)

    def test_distortion(self):
        assert_array_less(np.abs(self.cam.distortion - self.calib_results['kc']), self.calib_results['kc_error'] + self.eps)

