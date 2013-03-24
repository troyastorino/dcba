import unittest

from scan.util import *
import os.path

class TestUtil(unittest.TestCase):
    def test_file_dir(self):
        # just checking that it works as expected with __file__
        self.assertEqual(file_dir(__file__), os.path.dirname(os.path.realpath(__file__)))

    def test_rel_file(self):
        # also just checking that it works as expected with __file__
        rel_path = os.path.join("path", "to", "blah")
        self.assertEqual(rel_to_file(rel_path, __file__), os.path.join(file_dir(__file__), rel_path))
