import unittest
import numpy as np
from numpy.testing import assert_equal, assert_allclose
from scan.triangulation.math import *
from scan.common.math import normalize, rotation_matrix
from scan.data_capture.optical_equipment import OpticalEquipment

class TestPixelRays(unittest.TestCase):
    def setUp(self):
        self.rtol = 1e-7
        R = np.eye(3)
        T = np.zeros(3)
        intrinsic_matrix = np.eye(3)
        distortion = np.zeros(5)
        self.resolution = (4,3)
        self.oe = OpticalEquipment(OpticalEquipment.extrinsic_matrix(R, T),
                                   intrinsic_matrix, distortion, self.resolution)
    def test_pixel_generation(self):
        unnormalized_pixel_rays = np.transpose(
            [[[0, 0, 1], [1, 0, 1], [2, 0, 1], [3, 0, 1]],
             [[0, 1, 1], [1, 1, 1], [2, 1, 1], [3, 1, 1]],
             [[0, 2, 1], [1, 2, 1], [2, 2, 1], [3, 2, 1]]],
            (1, 0, 2))
        assert_allclose(pixel_rays(self.oe), normalize(unnormalized_pixel_rays), rtol=self.rtol)

class TestGlobalFrameTransformation(unittest.TestCase):
    def setUp(self):
        self.rtol = 1e-7
        self.atol = 1e-7        
        np.random.random(3,) - 0.5
        self.R = rotation_matrix(np.random.random(3,) - 0.5, np.random.rand() * 2 * np.pi)
        self.T = np.random.random((3,))
        extrinsic_matrix = OpticalEquipment.extrinsic_matrix(self.R, self.T)
        self.oe = OpticalEquipment(extrinsic_matrix, np.eye(3), None, (1600, 1200))

    def hard_coded_rotation_test(self):
        x = [1, 0, 0]
        x_global = [1/np.sqrt(2), 0, 1/np.sqrt(2)]
        theta = np.radians(45)
        R = [[np.cos(theta), 0, np.sin(theta)],
             [0, 1, 0],
             [-np.sin(theta), 0, np.cos(theta)]]
        T = np.zeros(3)
        extrinsic_matrix = OpticalEquipment.extrinsic_matrix(R, T)
        oe = OpticalEquipment(extrinsic_matrix, np.eye(3), None, (32, 14))

        assert_allclose(to_global_frame(x, oe), x_global, rtol=self.rtol)
        
    def hard_coded_translation_test(self):
        x = [0, 0, 0]
        x_global = [1, 1, -1]
        R = np.eye(3)
        T = [-1, -1, 1]
        extrinsic_matrix = OpticalEquipment.extrinsic_matrix(R, T)
        oe = OpticalEquipment(extrinsic_matrix, np.eye(3), None, (32, 14))

        assert_allclose(to_global_frame(x, oe), x_global, rtol=self.rtol)
        
    def hard_coded_translation_and_rotation_test(self):
        x = [0, 0, 0]
        x_global = [1, 0, 0]
        theta = np.radians(45)
        R = [[np.cos(theta), 0, np.sin(theta)],
             [0, 1, 0],
             [-np.sin(theta), 0, np.cos(theta)]]
        T = [-1/np.sqrt(2), 0, 1/np.sqrt(2)]
        extrinsic_matrix = OpticalEquipment.extrinsic_matrix(R, T)
        oe = OpticalEquipment(extrinsic_matrix, np.eye(3), None, (32, 14))

        assert_allclose(to_global_frame(x, oe), x_global, rtol=self.rtol, atol=self.atol)
        
    def test_one_point(self):
        x = 100 * np.random.random((3,)) - 50
        x_global = np.dot(self.R.T, x - self.T)
        assert_allclose(to_global_frame(x, self.oe), x_global, rtol=self.rtol)
        
    def test_list_of_points(self):
        shape = (10,3)
        x = 10 * np.random.random(shape) - 5
        x_global = np.zeros(shape)
        for i in range(shape[0]):
            x_global[i] = to_global_frame(x[i], self.oe)
        assert_allclose(to_global_frame(x, self.oe), x_global, rtol=self.rtol)

    def test_grid_of_points(self):
        shape = (5, 8, 3)
        x = 10 * np.random.random(shape) - 5
        x_global = np.zeros(shape)
        for i in range(shape[0]):
            for j in range(shape[1]):
                x_global[i,j] = to_global_frame(x[i,j], self.oe)
        assert_allclose(to_global_frame(x, self.oe), x_global, rtol=self.rtol)
        
class TestLinePlaneIntersections(unittest.TestCase):
    def test_single_intersection(self):
        q = [0,0,0]
        v = [1,0,0]
        w = [1,0,0,2]

        p = [2,0,0]

        assert_allclose(normalize(line_plane_intersection(q, v, w)), normalize(p))
