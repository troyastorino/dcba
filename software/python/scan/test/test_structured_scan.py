import unittest
import scipy.io

class TestStructuredLightScan(unittest.TestCase):
    def setUp():
        self.structured_light_dir = rel_to_file("test-data", "structured-light", __file__)
        self.calib = scipy.io.loadmat(os.path.join(self.structured_light_dir, "calib",
                                                   "calib_results", "calib_cam_proj.mat"))
