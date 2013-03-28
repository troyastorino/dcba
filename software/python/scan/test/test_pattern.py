import unittest
from numpy.testing import assert_equal
from scan.data_capture.pattern import *

class TestGrayCodePatterns(unittest.TestCase):
    def test_vertical_stripes(self):
        height = 6
        img_size = (height,4)

        patterns = [
            DLPPattern(np.tile([0, 0, 255, 255], (height,1))),
            DLPPattern(np.tile([255, 255, 0, 0], (height,1))),
            DLPPattern(np.tile([0, 255, 255, 0], (height,1))),
            DLPPattern(np.tile([255, 0, 0, 255], (height,1)))]

        generated_patterns = gray_code_patterns(img_size)
        
        for i in range(len(patterns)):
            assert_equal(generated_patterns[i].image, patterns[i].image)
            
    def test_horizontal_stripes(self):
        width = 12
        img_size = (4,width)

        patterns = [
            DLPPattern(np.tile([0, 0, 255, 255], (width,1)).T),
            DLPPattern(np.tile([255, 255, 0, 0], (width,1)).T),
            DLPPattern(np.tile([0, 255, 255, 0], (width,1)).T),
            DLPPattern(np.tile([255, 0, 0, 255], (width,1)).T)]

        generated_patterns = gray_code_patterns(img_size, vertical_stripes=False)
        
        for i in range(len(patterns)):
            assert_equal(generated_patterns[i].image, patterns[i].image)

class TestAreInverses(unittest.TestCase):
    def test_are_inverses_false(self):
        x = DLPPattern(np.array([255, 0, 0]))
        y = DLPPattern(np.array([0, 0, 255]))
        self.assertFalse(are_inverses(x, y))

    def test_are_inverses(self):
        x = DLPPattern(np.array([[0, 127],
                                 [0, 0],
                                 [0, 127]]))
        y = DLPPattern(np.array([[127, 0],
                                 [127, 127],
                                 [127, 0]]))
        self.assertTrue(are_inverses(x, y))
