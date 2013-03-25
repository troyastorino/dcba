import unittest
import scipy.io
import os.path
from scan.data_capture.image import load_from_directory
from scan.data_capture.calibration import calibrate_with_checkerboard
from scan.util import rel_to_file

# get path to current directory


class TestCameraCalibration(unittest.TestCase):
    def setUp(self):
        # set up input data dir, parameters
        self.interior_corners = (8,6)
        self.side_length = 1.8

        # use results from matlab script to test
        self.calib_folder = rel_to_file("test-data/calib/calib", __file__)
        self.calib_images = load_from_directory(self.calib_folder)
        self.calib_results = scipy.io.loadmat(os.path.join(rel_to_file, "Calib_Results.mat"))

    def test_calibration(self):
        # run the calibration
        cam = calibrate_with_checkerboard(None, self.calibration_images, self.interior_corners, self.side_length)

        # make sure results agree with those from the matlab script TODO do this
        self.assertTrue(True)
