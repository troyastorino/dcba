import unittest
import scipy.io
import os.path
import numpy as np
from numpy.testing import assert_allclose
from nose.tools import nottest
from nose.plugins.attrib import attr
from scan.data_capture.image import Image, load_from_directory
from scan.data_capture.optical_equipment import Camera, DLPProjector, OpticalEquipment
from scan.common.util import rel_to_file
from scan.common.math import fit_plane, normalize
from scan.data_capture.pattern import gray_code_patterns, GeneratedPattern, DLPPattern
from scan.triangulation.gray_scan import *
from scan.visualization.point_cloud import view_point_cloud

class TestProjectorPlanes(unittest.TestCase):
    def setUp(self):
        self.rtol = 1e-7
        self.atol = 1e-7
        R = np.eye(3)
        T = np.zeros(3)
        extrinsic_matrix = OpticalEquipment.extrinsic_matrix(R, T)
        intrinsic_matrix = np.eye(3)
        distortion = np.zeros(5)
        resolution = (3,2)
        oe = DLPProjector(extrinsic_matrix, intrinsic_matrix,
                          distortion, resolution)
        self.row_planes, self.col_planes = projector_planes(oe)
        pixels = np.array([[[0, 0], [0, 1]],
                           [[1, 0], [1, 1]],
                           [[2, 0], [2, 1]]])
        self.exp_row_planes = [[1, 0, 0, 0],
                               fit_plane([[1, 0, 1],[1, 1, 1],[0, 0, 0]]),
                               fit_plane([[2, 0, 1],[2, 1, 1],[0, 0, 0]])]
        self.exp_col_planes = [[0, 1, 0, 0],
                               fit_plane([[0, 1, 1],[1, 1, 1],[2, 1, 1],[0, 0, 0]])]

    def test_row_planes(self):
        assert_allclose(normalize(np.around(self.row_planes, decimals=10)),
                        normalize(np.around(self.exp_row_planes, decimals=10)),
                        rtol=self.rtol, atol=self.atol)

    def test_col_planes(self):
        assert_allclose(normalize(np.around(self.col_planes, decimals=10)),
                        normalize(np.around(self.exp_col_planes, decimals=10)),
                        rtol=self.rtol, atol=self.atol)

@attr('slow')
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
        cam_resolution = (cls.calib['nx_cam'], cls.calib['ny_cam'])
        cls.cam = Camera(None, cam_extrinsic_matrix, cam_intrinsic_matrix, cam_distortion, cam_resolution)

        # Initialize the projector
        proj_extrinsic_matrix = DLPProjector.extrinsic_matrix(cls.calib['Rc_1_proj'],
                                                           cls.calib['Tc_1_proj'])
        proj_intrinsic_matrix = DLPProjector.intrinsic_matrix(cls.calib['fc_proj'],
                                                           cls.calib['cc_proj'],
                                                           cls.calib['alpha_c_proj'])
        proj_distortion = cls.calib['kc_proj']
        proj_resolution = (cls.calib['nx_proj'], cls.calib['ny_proj'])
        cls.proj = DLPProjector(proj_extrinsic_matrix, proj_intrinsic_matrix, proj_distortion, proj_resolution)

        # Get the Images
        img_folder = os.path.join(structured_light_dir, "data", "Gray", "man", "v1")
        loaded_images = load_from_directory(img_folder)

        # Get the Gray code patterns that were projected
        pattern_shape = (cls.calib['nx_proj'], cls.calib['ny_proj'])
        patterns = [DLPPattern(np.ones(pattern_shape, dtype=np.uint8) * 255),
                          DLPPattern(np.zeros(pattern_shape, dtype=np.uint8))]
        patterns.extend(gray_code_patterns(pattern_shape))
        patterns.extend(gray_code_patterns(pattern_shape, vertical_stripes=False))

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

        # finally at the point we would have been at if our machine scanned the
        # object :)
        cls.point_cloud = extract_point_cloud(images)

        view_point_cloud(cls.point_cloud)

    def test_scan_results(self):
        raise NotImplementedError()
