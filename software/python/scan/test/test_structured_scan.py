import unittest
import scipy.io
import os.path
import numpy as np
from scan.data_capture.optical_equipment import Camera, DLPProjector
from scan.util import rel_to_file

class TestStructuredLightScan(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # get the relevant data
        structured_light_dir = rel_to_file(os.path.join("test-data", "structured-light"),
                                               __file__)
        cls.calib = scipy.io.loadmat(os.path.join(structured_light_dir, "calib",
                                                  "calib_results", "calib_cam_proj.mat"),
                                      squeeze_me=True)


        # Initialize the camera
        cam_extrinsic_matrix = Camera.extrinsic_matrix(cls.calib['Rc_1_cam'],
                                                       cls.calib['Tc_1_cam'])
        cam_intrinsic_matrix = Camera.intrinsic_matrix(cls.calib['fc_cam'],
                                                       cls.calib['cc_cam'],
                                                       cls.calib['alpha_c_cam'])
        cam_distortion = cls.calib['kc_cam']
        cls.cam = Camera(None, cam_extrinsic_matrix, cam_intrinsic_matrix, cam_distortion)

        # Initialize the projector
        proj_extrinsic_matrix = DLPProjector.extrinsic_matrix(cls.calib['Rc_1_proj'],
                                                           cls.calib['Tc_1_proj'])
        proj_intrinsic_matrix = DLPProjector.intrinsic_matrix(cls.calib['fc_proj'],
                                                           cls.calib['cc_proj'],
                                                           cls.calib['alpha_c_proj'])
        proj_distortion = cls.calib['kc_proj']
        cls.proj = DLPProjector(proj_extrinsic_matrix, proj_intrinsic_matrix, proj_distortion)

    def test_scan_results(self):
        raise NotImplementedError()
