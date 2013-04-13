import pcl
from time import sleep

def view_point_cloud(cloud, window_name="Point Cloud Visualization"):
    """
    Function view_point_cloud
    Takes a point cloud and displays it on the PCL CloudViewer
    """
    viewer = pcl.PCLVisualizer(window_name)
    viewer.add_point_cloud(cloud)

    while not viewer.was_stopped():
        viewer.spin_once(100)
        sleep(0.1)
