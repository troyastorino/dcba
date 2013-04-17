import pcl
import numpy as np
from scan.visualization.point_cloud import view_point_cloud
from scan.visualization.image import view_image
from scan.data_capture.image import load_image

def test_visualization_of_cylinder_point_cloud():
    z = np.r_[-1:1:.05]
    n_z = len(z)
    angle = np.r_[0:360:5]
    n_a = len(angle)
    x = np.cos(np.radians(angle))
    y = np.sin(np.radians(angle))
    points = np.empty((n_z*n_a, 3))

    for i in range(n_z):
        loc = n_a * i
        new_points = np.ones((n_a, 3)) * z[i]
        new_points[:,0] = x
        new_points[:,1] = y
        points[loc:loc+n_a] = new_points

    pc = pcl.PointCloud()
    pc.from_array(points.astype('float32'))

    view_point_cloud(pc, timeout=1)

def test_view_image():
    structured_light_dir = rel_to_file(os.path.join("test-data", "structured-light"),
                                       __file__)
    img_path = os.path.join(structured_light_dir, "data", "Gray", "man", "v1", "10.bmp")
    img = load_image(img_path)

    view_image(img, timeout=1)
