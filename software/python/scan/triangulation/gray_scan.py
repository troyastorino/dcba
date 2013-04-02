import numpy as np
import cv2
from scan.data_capture.pattern import are_inverses
from scan.triangulation.math import pixel_rays, line_plane_intersection, to_global_frame
from scan.common.math import pack_bit_array, from_gray_code, fit_plane

def gray_code_estimates(images, min_contrast=0.2):
    """
    Function: extract_point_cloud
    Takes a series of images with gray codes projected on them and calculates the
    gray code value that was projected for each camera pixel
    
    Parameters:
    images - *[<Image>]* The list of images taken with a gray code patterns
    projected on the object. Must be ordered with each image followed by its
    inverse projection. On the first image of an inverse pair, black corresponds
    to a gray code bit of 1, and white corresponds to a gray code bit of 1. For
    instance, the first image will have the first half black and the second half
    white, the second will have the first half white and the second half black,
    etc.
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
    valid_pixel_mask = np.zeros(img_size[0:2], dtype=np.bool)
    gray_code_bits = np.zeros(img_size[0:2] + (len(images)/2,), dtype=np.uint8)
    
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

    # convert gray code bits to a decimal representation of the the gray code patterns
    gray_code = pack_bit_array(gray_code_bits)

    # convert from the gray code numbers to decimal numbers
    dec_array = from_gray_code(gray_code)

    return dec_array, valid_pixel_mask

def projector_planes(proj):
    """
    Function: projector_planes
    Generates the planes representing each row and column of the projector

    Parameters:
    proj - *<DLPProjector>* The projector to generate the planes of

    Returns:
    *Tuple* in the form (row_planes, col_planes). row_planes will have shape
    (width, 4) and col_planes will have shape (height, 4).  Each plane is defined
    by the coefficients [a, b, c, d], where a*x + b*y + c*z = d
    """
    # generate pixel rays in the projector frame
    proj_pixels_proj_frame = pixel_rays(proj)
    
    # translate and rotate the rays into the global frame
    proj_pixels = to_global_frame(proj_pixels_proj_frame, proj)

    # get projector location in the global frame
    proj_pose = to_global_frame([0, 0, 0], proj)

    # add projector location to the row points
    row_shape = (proj.resolution[0], 1, 3)
    proj_points_row = np.concatenate((proj_pixels, np.ones(row_shape) * proj_pose), axis=1)
    
    # calculate the row planes
    proj_planes_row = fit_plane(proj_points_row)

    # add projector location to the column points
    col_shape = (1, proj.resolution[1], 3)
    proj_points_col = np.concatenate((proj_pixels, np.ones(col_shape) * proj_pose), axis=0)

    # calculate the column planes
    proj_planes_col = fit_plane(np.transpose(proj_points_col, (1, 0, 2)))

    return proj_planes_row, proj_planes_col
    
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
    *ndarray* A list of points representing object points in 3D space. Will
    have shape (n, 3)
    """
    if len(images) % 2 != 0:
        raise Exception("Must have an even number of images, because for every " +
                        "image must have an image with an inverse pattern")
    if (len(images) - 2) % 4 != 0:
        raise Exception("Must have an all white projection image and an all black " +
                        "projecting image, followed by a vertical stripe gray code" +
                        "image sequence and a horizontal strip gray code image " +
                        "sequence of equal lengths.")
    
    # find gray code sequence length
    n = (len(images) - 2) / 4

    # get row projection indices from vertical stripe pattenrs
    gray_code_row, pixel_mask_row = gray_code_estimates(images[2:2+n], min_contrast)

    # get column projection indices from horizontal stripe patterns
    gray_code_col, pixel_mask_col = gray_code_estimates(images[2+n:], min_contrast)

    # combine pixel masks from columns and rows (i.e., if a pixel was invalid
    # from the column or row data, make it invalid for both)
    pixel_mask = pixel_mask_row & pixel_mask_col

    # also invalidate pixels if difference between original first and second
    # image (black projection and whit projetion) doesn't exceed contrast
    # ratio
    gray_1 = cv2.cvtColor(images[0].data, cv2.COLOR_RGB2GRAY)
    gray_2 = cv2.cvtColor(images[1].data, cv2.COLOR_RGB2GRAY)
    pixel_mask[np.abs(gray_1 - gray_2) <= 255 * min_contrast] = False

    # alias projector and camera objects
    proj = images[0].patterns[0].projected_patterns[0][1]
    cam = images[0].camera
    
    # also invalidate pixel if any the calculated gray code row is greater than
    # the projection row, or if the calculated gray code column is greater than
    # the gray code column
    pixel_mask[gray_code_row >= proj.resolution[0]] = False
    pixel_mask[gray_code_col >= proj.resolution[1]] = False

    # get plane equations for every projector row and column
    proj_planes_row, proj_planes_col = projector_planes(proj)

    # get camera pixel rays
    cam_rays = to_global_frame(pixel_rays(cam), cam)

    # get cam location
    cam_pose = to_global_frame([0, 0, 0], cam)

    # TODO: vectorize this next part
    # calculate all the points
    points = []
    for i in range(cam.resolution[0]):
        for j in range(cam.resolution[1]):
            if pixel_mask[i,j]:
                p_row = line_plane_intersection(cam_pose, cam_rays[i,j],
                                                proj_planes_row[gray_code_row[i,j]])
                p_col = line_plane_intersection(cam_pose, cam_rays[i,j],
                                                proj_planes_col[gray_code_col[i,j]])
                points.append(np.average((p_row, p_col), axis=0))

    return np.array(points)
