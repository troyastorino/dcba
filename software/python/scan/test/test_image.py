import unittest
import numpy as np
from numpy.testing import assert_equal
from scan.data_capture.image import *

class TestOpenCVConversions(unittest.TestCase):
    def setUp(self):
        height = 9
        width = 16
        self.img = np.random.random((width, height, 3))
        self.img_obj = Image(self.img, None, None)
        self.cv_img = np.transpose(self.img, (1, 0, 2))

    def test_to_opencv(self):
        assert_equal(to_OpenCV(self.img), self.cv_img)

    def test_image_obj_to_opencv(self):
        assert_equal(to_OpenCV(self.img_obj), self.cv_img)

    def test_from_opencv(self):
        assert_equal(from_OpenCV(self.cv_img), self.img)

    def test_opencv_conversion_consistency(self):
        img = np.random.random((1024, 768))
        assert_equal(to_OpenCV(from_OpenCV(img)), img)
    
    
        
