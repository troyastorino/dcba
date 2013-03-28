import unittest
from numpy.testing import assert_equal
from scan.data_capture.pattern import *

class TestGrayCodePatterns(unittest.TestCase):
    def test_vertical_patterns(self):
        img_size = (1,4)

        patterns = [
            DLPPattern(np.array([[255, 255, 255, 255]])),
            DLPPattern(np.array([[0, 0, 0, 0]])),
            DLPPattern(np.array([[0, 0, 255, 255]])),
            DLPPattern(np.array([[255, 255, 0, 0]])),
            DLPPattern(np.array([[0, 255, 255, 0]])),
            DLPPattern(np.array([[255, 0, 0, 255]]))]

        generated_patterns = gray_code_patterns(img_size)
        
        for i in range(len(patterns)):
            assert_equal(generated_patterns[i].image, patterns[i].image)
            
    def test_horizontal_patterns(self):
        img_size = (4,1)

        patterns = [
            DLPPattern(np.array([[255, 255, 255, 255]]).T),
            DLPPattern(np.array([[0, 0, 0, 0]]).T),
            DLPPattern(np.array([[0, 0, 255, 255]]).T),
            DLPPattern(np.array([[255, 255, 0, 0]]).T),
            DLPPattern(np.array([[0, 255, 255, 0]]).T),
            DLPPattern(np.array([[255, 0, 0, 255]]).T)]

        generated_patterns = gray_code_patterns(img_size, vertical_stripes=False)
        
        for i in range(len(patterns)):
            assert_equal(generated_patterns[i].image, patterns[i].image)
            
