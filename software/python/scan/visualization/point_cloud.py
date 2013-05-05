import pcl
from threading import Thread
import time

def view_point_cloud(cloud, window_name="Point Cloud Visualization", timeout=None):
    """
    Function view_point_cloud
    Takes a point cloud and displays it on the PCL CloudViewer

    Parameters:
    cloud - *PointCloud* Point cloud to display
    window_name - *string* Name of the window to hold the visualization
    timeout - *int* Approximate number of seconds to keep viewer open before automatically closing it. If None never times out.  Defaults to None
    """
    if timeout:
        raise NotImplementedError("can't get the timeout to work because was_stopped doesn't seem to work and the PCLVisualizer apperantly needs to be in the main thread")

    # initialize the viewer
    viewer = pcl.PCLVisualizer(window_name)
    viewer.init_camera_parameters()
    viewer.add_point_cloud(cloud)
    viewer.add_coordinate_system()

    # allow interactions and wait for the viewer to close
    viewer.spin()

    # make sure the viewer closes
    viewer.close()
