import numpy as np
import cv2
from scan.data_capture.pattern import are_inverses

def gray_code_estimates(images):
    """
    Function: extract_point_cloud
    Takes a series of images with gray codes projected on them and calculates the
    gray code value that was projected for each camera pixel
    
    Parameters:
    images - *[<Image>]* The list of images taken with a gray code patterns projected
    on the object. Must be ordered with each image followed by its inverse projection.
    The first image should be an all white projection, the second all black, the third
    will have the first half black and the second half white, the fourth will have the
    first half white and the second half black, etc.
    min_contrast - contrast threshold for a pixel between images with a projection and
    its inverse projection.  If the contrast between those two images is less than 0,
    it is not marked as valid in the pixel mask

    Returns:
    
    Tuple of (gray_code_values, valid_pixel_mask), where each of those arrays
    have the same shape as the images passed in. gray_code_values has decimal
    values at each pixel location corresponding to the projector row or column
    that was projected onto that pixel (depending on whether it was horizontal
    or vertical stripes that were projected) and valid_pixel_mask tells whether
    all the images at that pixel exceeded the contrast threshold between the
    inverse images
    """
    if len(images) % 2 != 0:
        raise Exception("Must have an even number of images, because for every image must have an image with an inverse pattern")

    img_size = images[0].data.shape

    # structures to fill through loop
    valid_pixel_mask = np.zeros(img_size, dtype=np.bool)
    gray_code_bits = np.zeros(list(img_size) + [n_pairs])
    
    # go through image pairs
    for i in range(len(images) / 2):
        # make sure the projected patterns were inverse patterns
        if not are_inverses(images[2*i].patterns[0].projected_patterns[0][0],
                            images[2*i+1].patterns[0].projected_patterns[0][0]):
            raise Exception("The images must be ordered such that each set of " +
                            "two images are inverses of each other")
        
        # Convert images to grayscale
        gray_1 = cv2.cvtColor(images[2*i].data, cv2.COLOR_RGB2GRAY)
        gray_2 = cv2.cvtColor(images[2*i+1].data, cv2.COLOR_RGB2GRAY)

        # check which pixels exceed the contrast ratio
        valid_pixel_mask[abs(gray_1 - gray_2) > 255 * min_contrast] = True

        # set bit of gray code for each image
        gray_code_bits[gray_1 >= gray_2, i] = 1

    # convert from gray code bit array to decimals
    return pack_bit_array(gray_code_bits), valid_pixel_mask
    
def extract_point_cloud(images, min_contrast=0.2):
    """
    Function: extract_point_cloud
    Takes a series of images with gray codes projected on them and produces a point cloud.
    
    Parameters:
    images - *[<Image>]* The list of images taken with a gray code patterns
    projected on the object. The first image must be of an all white projection,
    and the second must be an all black projection. This must be followed by a
    set of vertical stripe images, and then a set of horizontal strip images.
    The stripe image patterns must be the way described in <gray_code_estimates>

    Returns:
    point cloud
    """
    if len(images) % 2 != 0:
        raise Exception("Must have an even number of images, because for every image must have an image with an inverse pattern")

    
        
