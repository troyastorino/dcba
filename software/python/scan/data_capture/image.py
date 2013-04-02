import os
import cv2
import numpy as np

class Image:
    """
    Class: Image
    Stores all the information relevant to one captured image from the
    environment. This includes the patterns projected on the object when the
    image was taken, the camera that took the image, and the image data itself,
    in matrix form.

    Properties:
    patterns - *[<GeneratedPattern>]* Light patterns projected on
    the object when the image was taken
    camera - *<Camera>* The camera that the image was taken with
    data - *ndarray* The pixel data comprising the image
    corrected - *boolean* Whether or not the image data has been
    corrected for the distortion of the camera
    """
    def __init__(self, data, camera, patterns, corrected=False):
        self.data = data
        self.camera = camera
        self.patterns = patterns
        self.corrected = corrected

def to_OpenCV(img):
    """
    Function: to_OpenCV
    Takes an <Image> or a *ndarray* and transposes the image data from the
    convention used in our code (width, height) to the convention used by
    OpenCV (height, width)

    Parameters:
    img - *<Image>* or *ndarray* The image object or image array to be transformed

    Returns
    *ndarray* The image data in the form OpenCV expects
    """
    if issubclass(img.__class__, Image):
        img = img.data

    n_dims = len(img.shape)

    return np.transpose(img, [1, 0] + range(2, n_dims))

def from_OpenCV(img):
    """
    Function: to_OpenCV
    Takes an image *ndarray* and transposes the array from the convention used
    by OpenCV (height, width) to the convention used in our code (width, height)

    Parameters:
    img - *ndarray* The image array to be transformed

    Returns
    *ndarray* The image data in the form our code expects it
    """
    n_dims = len(img.shape)

    return np.transpose(img, [1, 0] + range(2, n_dims))

def load_image(path, camera=None, patterns=None):
    """
    Function: load_images
    Loads an image file.

    Parameters:
    path - *string* Path where the image is located
    camera - *<Camera>* the image was taken with. Defaults to None
    patterns - *[<GeneratedPattern>]* Light patterns projected on
    the object when the image was taken. Defaults to None

    Throws:
    *Exception* if loading the image fails

    Returns:
    *<Image>* object
    """
    cv_data = cv2.imread(path)

    if cv_data == None:
        raise Exception('Could not load image')
    
    data = from_OpenCV(cv_data)

    return Image(data, camera, patterns)
    
def load_from_directory(dir_name, camera=None, patterns=None):
    """
    Function: load_from_directory
    Loads all of the image files in a directory and returns them as a list of <Image>s

    Parameters:
    string dir_name - The name of the directory
    patterns - *[<GeneratedPattern>]* Light patterns projected on
    the object when the images were taken. Defaults to None
    camera - *<Camera>* The camera that the images were taken with. Defaults to None

    Returns:
    A list of <Image>s read from the directory. Images are sorted by filename

    """
    # list of images to return
    images = []

    # try to add each item in the directory as an image
    files = os.listdir(dir_name)
    files.sort()
    for f in files:
        # append the image to the list. if loading an image failes, assume it
        # isn't an image
        try:
            images.append(load_image(os.path.join(dir_name, f)))
        except:
            continue
    # return the list of images
    return images
