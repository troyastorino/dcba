import cv2
import time
from scan.data_capture.image import to_OpenCV

def view_image(img, window_name="Image Viewer", timeout=None):
    """
    Function view_image:
    Creates a window to show an image in.  Closes the window when a key is pressed or the timeout finishes

    Parameters:
    img - *<Image>* image to show
    window_name - *string* name of the window to show the image in
    timeout - *float* Approximate number of seconds to wait before
    """
    cv2.namedWindow(window_name)

    cv2.imshow(window_name, to_OpenCV(img))

    def timed_out(time_elapsed):
        if not timeout:
            return False
        else:
            return time_elapsed >= timeout

    time_elapsed = 0 # in seconds
    delay_time = 10 # in milliseconds
    while True:
        if cv2.waitKey(delay_time) > 0 or timed_out(time_elapsed):
            cv2.destroyWindow(window_name)
            break
        if timeout:
            time_elapsed = time_elapsed + delay_time / 1000.0
