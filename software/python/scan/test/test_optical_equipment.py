import unittest
import numpy as np
from numpy.random import random
from numpy.testing import assert_equal
from scan.data_capture.optical_equipment import OpticalEquipment

class TestIntrinsicParamConversionConsistency(unittest.TestCase):
    def setUp(self):
        self.focal_length = random((2,))
        self.alpha = random()
        self.principal_point = random((2,))
        self.calculated_params = OpticalEquipment.intrinsic_parameters(
            OpticalEquipment.intrinsic_matrix(self.focal_length, self.principal_point, self.alpha))

    def test_focal_length(self):
        assert_equal(self.calculated_params[0], self.focal_length)

    def test_principal_point(self):
        assert_equal(self.calculated_params[1], self.principal_point)

    def test_alpha(self):
        self.assertEqual(self.calculated_params[2], self.alpha)        

class TestExtrinsicParamConversionConsistency(unittest.TestCase):
    def setUp(self):
        self.R = random((3,3))
        self.T = random((3,))
        self.calculated_params = OpticalEquipment.extrinsic_parameters(
            OpticalEquipment.extrinsic_matrix(self.R, self.T))

    def test_R(self):
        assert_equal(self.calculated_params[0], self.R)

    def test_T(self):
        assert_equal(self.calculated_params[1], self.T)
