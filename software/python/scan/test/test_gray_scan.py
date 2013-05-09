import unittest
import scipy.io
import os.path
import numpy as np
import pcl
from numpy.testing import assert_allclose, assert_equal
from nose.tools import nottest
from nose.plugins.attrib import attr
from scan.data_capture.image import Image, load_from_directory
from scan.data_capture.optical_equipment import Camera, DLPProjector, OpticalEquipment
from scan.common.util import rel_to_file
from scan.common.math import fit_plane, normalize
from scan.data_capture.pattern import gray_code_patterns, GeneratedPattern, DLPPattern
from scan.triangulation.gray_scan import *
from scan.visualization.point_cloud import view_point_cloud
from scan.visualization.image import grayscale_to_RGB

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

class TestProjPlanesRotated(unittest.TestCase):
    def setUp(self):
        self.rtol = 1e-7
        self.atol = 1e-7
        theta = np.radians(45)
        R = [[np.cos(theta), 0, np.sin(theta)],
             [0, 1, 0],
             [-np.sin(theta), 0, np.cos(theta)]]
        T = [-np.cos(theta), 0, np.sin(theta)]
        extrinsic_matrix = OpticalEquipment.extrinsic_matrix(R, T)
        intrinsic_matrix = np.eye(3)
        distortion = np.zeros(5)
        resolution = (2,2)
        oe = DLPProjector(extrinsic_matrix, intrinsic_matrix,
                          distortion, resolution)
        self.vert_planes, self.horz_planes = projector_planes(oe)
        self.exp_vert_planes = [fit_plane([[1, 0, 0], [1, 1, 0], [0, 0, 1]]),
                                [1, 0, 0, 1]]
        # find where z=1 in the projector plane for the x=0 points
        x0 = 1 - 1/np.sqrt(2)
        z0 = 1/np.sqrt(2)
        self.exp_horz_planes = [[0, 1, 0, 0],
                               fit_plane([[1, 0, 0], [x0, 1, z0], [1, 1, np.sqrt(2)]])]

    def test_vert_planes(self):
        assert_allclose(normalize(np.around(self.vert_planes, decimals=10)),
                        normalize(np.around(self.exp_vert_planes, decimals=10)),
                        rtol=self.rtol, atol=self.atol)

    def test_horz_planes(self):
        assert_allclose(normalize(np.around(self.horz_planes, decimals=10)),
                        normalize(np.around(self.exp_horz_planes, decimals=10)),
                        rtol=self.rtol, atol=self.atol)

class TestGrayCodeEstimates(unittest.TestCase):
    def setUp(self):
        data = [[[8, 240, 12, 225],
                 [240, 12, 225, 12]],
                [[234, 12, 25, 8],
                 [12, 25, 8, 250]],
                [[12, 219, 248, 21],
                 [219, 248, 21, 8]],
                [[243, 5, 8, 252],
                 [174, 8, 252, 230]]]
        patterns = gray_code_patterns((2,4), vertical_stripes=False)

        images = []
        for i in range(len(data)):
            images.append(Image(grayscale_to_RGB(data[i]), None, [GeneratedPattern([(patterns[i], None)])]))

        (self.gray_code_values, self.valid_pixel_mask) = gray_code_estimates(images)

    def test_gray_code_estiamtes(self):
        expected_nums = [[0, 2, 1, 3],
                         [2, 1, 3, 0]]

        assert_equal(self.gray_code_values, expected_nums)

    def test_pixel_mask(self):
        pixel_mask = [[True, True, False, True],
                      [False, False, True, True]]

        assert_equal(self.valid_pixel_mask, pixel_mask)


class TestVerticalStripeGrayCodeEstimates(unittest.TestCase):
    def setUp(self):
        self.width = 4
        self.height = 12
        patterns = gray_code_patterns((self.width, self.height))
        images = []
        for p in patterns:
            data = p.image
            images.append(Image(grayscale_to_RGB(data), None, [GeneratedPattern([(p, None)])]))
        (self.gray_code_values, self.valid_pixel_mask) = gray_code_estimates(images)

    def test_gray_code_estimates(self):
        expected_dec_numbers = np.tile(np.arange(self.width), (self.height, 1)).T

        assert_equal(self.gray_code_values, expected_dec_numbers)

    def test_valid_pixel_mask(self):
        valid_pixels = np.ones((self.width, self.height), dtype=np.bool)

        assert_equal(self.valid_pixel_mask, valid_pixels)

class TestHorizontalStripeGrayCodeEstimates(unittest.TestCase):
    def setUp(self):
        self.width = 4
        self.height = 16
        patterns = gray_code_patterns((self.width, self.height), vertical_stripes=False)
        images = []
        for p in patterns:
            data = p.image
            images.append(Image(grayscale_to_RGB(data), None, [GeneratedPattern([(p, None)])]))
        (self.gray_code_values, self.valid_pixel_mask) = gray_code_estimates(images)

    def test_gray_code_estimates(self):
        expected_dec_numbers = np.tile(np.arange(self.height), (self.width, 1))

        assert_equal(self.gray_code_values, expected_dec_numbers)

    def test_valid_pixel_mask(self):
        valid_pixels = np.ones((self.width, self.height), dtype=np.bool)

        assert_equal(self.valid_pixel_mask, valid_pixels)

class TestCalculatedGrayScan(unittest.TestCase):
    def setUp(self):
        self.cam = Camera(None, Camera.extrinsic_matrix(np.eye(3), np.zeros(3)),
                          Camera.intrinsic_matrix([1, 1], [1, 0]),
                          np.zeros(5), (3,1))
        self.proj = DLPProjector(DLPProjector.extrinsic_matrix(np.eye(3), [1, 0, 0]),
                                  DLPProjector.intrinsic_matrix([0.5, 1], [0, 0]),
                                  np.zeros(5), (2,2))

        # Get the Gray code patterns that were self.projected
        patterns = [DLPPattern(np.ones(self.proj.resolution, dtype=np.uint8) * 255),
                    DLPPattern(np.zeros(self.proj.resolution, dtype=np.uint8))]
        patterns.extend(gray_code_patterns(self.proj.resolution))
        patterns.extend(gray_code_patterns(self.proj.resolution, vertical_stripes=False))

        self.gen_patterns = []
        for p in patterns:
            self.gen_patterns.append([GeneratedPattern([(p, self.proj)])])

        self.expected_points = [[-1, 0, 1], [0, 0, 0.5], [1, 0, 1]]

    def gen_images(self, image_data):
        # reconstruct the images
        images = []

        for i in range(len(image_data)):
            images.append(Image(grayscale_to_RGB(image_data[i]), self.cam, self.gen_patterns[i]))

        return images

    def test_pure_images(self):
        image_data = [[[255], [255], [255]],
                      [[0], [0], [0]],
                      [[0], [255], [255]],
                      [[255], [0], [0]]]

        images = self.gen_images(image_data)

        point_cloud = extract_point_cloud(images[0:2], images[2:4], [])

        assert_allclose(point_cloud.to_array(), self.expected_points)

    def test_grayed_images(self):
        image_data = [[[186], [212], [169]],
                      [[10], [44], [38]],
                      [[12], [228], [234]],
                      [[176], [25], [41]]]

        images = self.gen_images(image_data)

        point_cloud = extract_point_cloud(images[0:2], images[2:4], [])

        assert_allclose(point_cloud.to_array(), self.expected_points)

@attr('test')
class TestCalculatedGrayScanVerticalStripes(unittest.TestCase):
    def setUp(self):
        self.cam = Camera(None, Camera.extrinsic_matrix(np.eye(3), [1, 0, 0]),
                          Camera.intrinsic_matrix([1, 1], [0, 0]),
                          np.zeros(5), (2,2))
        theta = np.radians(22.5)
        rot = [[np.cos(theta), 0, np.sin(theta)],
               [0, 1, 0],
               [-np.sin(theta), 0, np.cos(theta)]]
        self.proj = DLPProjector(DLPProjector.extrinsic_matrix(rot, [0, 0, 0]),
                                  DLPProjector.intrinsic_matrix([1, 1], [0, 0]),
                                  np.zeros(5), (2,2))

        # Get the Gray code patterns that were self.projected
        patterns = [DLPPattern(np.ones(self.proj.resolution, dtype=np.uint8) * 255),
                    DLPPattern(np.zeros(self.proj.resolution, dtype=np.uint8))]
        patterns.extend(gray_code_patterns(self.proj.resolution))

        self.gen_patterns = []
        for p in patterns:
            self.gen_patterns.append([GeneratedPattern([(p, self.proj)])])

        # calculate the vertical stripe point locations
        h1 = 1 / np.sin(theta)
        p1 = np.array([-1, 0, h1 * np.cos(theta)])

        h2 = np.sin(np.radians(45)) / np.sin(theta)
        p2 = np.array([h2 * np.sin(theta), 0, h2 * np.cos(theta)])

        p3 = p1.copy()
        p3[1] = p3[2]

        p4 = p2.copy()
        p4[1] = p4[2]
        
        self.expected_points = [p1, p2, p3, p4]

    def gen_images(self, image_data):
        # reconstruct the images
        images = []

        for i in range(len(image_data)):
            images.append(Image(grayscale_to_RGB(image_data[i]), self.cam, self.gen_patterns[i]))

        return images

    def test_pure_images(self):
        image_data = [[[255, 255],
                       [255, 255]],
                      [[0, 0],
                       [0, 0]],
                      [[0, 0],
                       [255, 255]],
                      [[255, 255],
                       [0, 0]]]

        images = self.gen_images(image_data)

        point_cloud = extract_point_cloud(images[0:2], images[2:4], [])

        assert_allclose(point_cloud.to_array(), self.expected_points)
        
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

        n = (len(images) - 2) / 2
            
        # finally at the point we would have been at if our machine scanned the
        # object :)
        cls.point_cloud = extract_point_cloud(images[0:2], images[2:2+n], images[2+n:])

        viewer = pcl.PCLVisualizer("View Gray Scan")
        viewer.add_point_cloud(cls.point_cloud)
        viewer.add_coordinate_system(translation=cls.proj.T, scale=10)
        viewer.add_text_3d("projector", cls.proj.T, text_id="projector")
        viewer.add_coordinate_system(translation=cls.cam.T, scale=10)
        viewer.add_text_3d("camera", cls.cam.T, text_id="camera")
        viewer.init_camera_parameters()

        viewer.spin()
        viewer.close()

    def test_scan_results(self):
        raise NotImplementedError()
