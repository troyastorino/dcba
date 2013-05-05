import cv2
import time
import numpy as np
from scan.data_capture.image import to_OpenCV

def view_image(img, window_name="Image Viewer", timeout=None):
    """
    Function view_image:
    Creates a window to show an image in.  Closes the window when a key is pressed or the timeout finishes

    Parameters:
    img - *<Image>* or *ndarray* image to show
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


def grayscale_to_RGB(image):
    """
    Function: pattern_to_RGB
    Takes a grayscale pattern and converts it to an RGB *ndarray*

    Parameters:
    image - *ndarray* grayscale pattern to convert to a color array

    Returns:
    *ndarray* RGB image data
    """
    image = np.array(image)

    if len(image.shape) > 2 and not (len(image.shape) == 3 and image.shape[2] == 1):
        raise Exception("image does not represent a valid grayscale image")

    return cv2.cvtColor(image.astype(np.uint8), cv2.COLOR_GRAY2RGB)
            