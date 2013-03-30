import unittest
import scipy.io
import os.path
import numpy as np
from scan.data_capture.image import Image, load_from_directory
from scan.data_capture.optical_equipment import Camera, DLPProjector
from scan.common.util import rel_to_file
from scan.data_capture.pattern import gray_code_patterns, GeneratedPattern, DLPPattern
from scan.triangulation.gray_scan import *

class TestGrayScan(unittest.TestCase):
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

        # Get the Images
        img_folder = os.path.join(structured_light_dir, "data", "Gray", "man", "v1")
        loaded_images = load_from_directory(img_folder)

        # Get the Gray code patterns that were projected
        # img_shape = loaded_images[0].data.shape[0:2] # it is hardcoded i
        img_shape = (768, 1024) # it is hard coded into the slProcess code...otherwise would just read the size like above
        patterns = [DLPPattern(np.ones(img_shape, dtype='i') * 255),
                          DLPPattern(np.zeros(img_shape, dtype='i'))]
        patterns.extend(gray_code_patterns(img_shape))
        patterns.extend(gray_code_patterns(img_shape, vertical_stripes=False))

        cls.gen_patterns = []
        for p in patterns:
            cls.gen_patterns.append([GeneratedPattern([(p, cls.proj)])])

        # make sure patterns and images are the same length
        if len(cls.gen_patterns) != len(loaded_images):
            raise Exception("Pattern and Image list should be the same length. Please figure out why they aren't")

        # reconstruct the images
        images = []
        for i in range(len(loaded_images)):
            images.append(Image(loaded_images[i].data, cls.cam, cls.gen_patterns[i]))

        point_cloud = extract_point_cloud(images)

        # now we are finally at the place we would be if our machine had done
        # the scanning on its own :)
        raise Exception()

    def test_scan_results(self):
        raise NotImplementedError()
