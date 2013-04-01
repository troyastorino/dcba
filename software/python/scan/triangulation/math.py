import numpy as np
import cv2
from scan.common.math import normalize

def pixel_rays(optical_equipment):
    """
    Function: pixel_rays
    Creates an array of the rays something with a given resolution

    Parameters:
    equipment - *<OpticalEquipment>* The piece of optical equipment to calculate
    the pixel rays for

    Returns:
    *ndarray* of the pixel rays for the piece of optical equipment.  Will
    have shape (height, width, 3), where height is the resolution height
    and width is the resolution width. The last dimension holds the ray
    """
    res = optical_equipment.resolution
    
    # get grid of pixel values
    distorted_pixels = np.transpose(np.mgrid[0:res[0], 0:res[1]], (1, 2, 0))

    # undistort pixels
    pixels = cv2.undistort(distorted_pixels.astype(np.float32),
                           optical_equipment.intrinsic_matrix,
                           optical_equipment.distortion)

    # convert to rays by appending 1 as z axis
    rays = np.concatenate((pixels, np.ones(res + (1,))), axis=2)

    # normalize
    return normalize(rays, axis=2)

def to_global_frame(x, optical_equipment, axis=None):
    """
    Function: to_global_frame
    Translates points from the coordinate frame of a piece of optical
    equipment into the global coordinate frame

    Parameters:
    x - *array-like* The points to translate to the global frame
    optical_equipment - *<OpticalEquipment>* Piece of optical equipment
    to use as the frame to translate from
    axis - The axis along which the points are located. Defaults to the
    second to last axis. The dim after axis must be of length 3
    """
    x = np.array(x)
    n_dim = len(x.shape)
    
    if axis == None:
        axis = n_dim - 2

    if x.shape[axis+1] != 3:
        raise Exception("Points must be 3 vectors in the dimension after the " +
                        "specified axis.")

    if axis != n_dim - 2:
        raise Exception("Not tested for specifying axis to anything other than the " +
                        "2nd to last dimension")

    # get the needed matrices
    R, T = optical_equipment.extrinsic_parameters(optical_equipment.extrinsic_matrix)
    
    # temporarily transpose the array to perform the operation
    if axis == -1:
        x_tmp = x
    else:
        point_axes = [axis, axis + 1]
        non_point_axes = [i for i in range(n_dim) if i not in point_axes]
        x_tmp = np.transpose(x, non_point_axes + point_axes)

    # compute R'*(x - t) to perform the transformation
    x_tmp_global_frame = np.einsum('...ji, ...j', R, x_tmp - T)
    
    # transpose back to the original dimensions
    if axis == -1:
        x_global_frame = x_tmp_global_frame
    else:
        axes = range(n_dim - 2)
        axes.insert(axis, n_dim - 2)
        axes.insert(axis + 1, n_dim - 1)
        x_global_frame = np.transpose(x_tmp_global_frame, axes)

    return x_global_frame

def line_plane_intersection(q, v, w):
    """
    Function: line_plane_intersection
    Calculates the intersection of line(s) and plane(s)

    Parameters:
    q - *array-like* Point(s) on the line. Last dim must hold the points
    v - *array-like* Vector(s) the line(s) span. Last dim must hold the vectors
    w - *array-like* Coefficients for the plane(s) being intersected. Last dim
    must hold the coefficients

    Returns:
    
    """
    # helper function to make the last axis the first axis
    def last_axis_to_first(x):
        x = np.array(x)
        return np.transpose(x, [len(x.shape)-1] + range(len(x.shape)-1))
    
    # temporarily transpose vectors to ease calculation notation
    q_trans = last_axis_to_first(q)
    v_trans = last_axis_to_first(v)
    w_trans = last_axis_to_first(w)

    # calculate scaling coefficient for spanning vector
    lmbda = ((w_trans[3] - np.einsum('i...,i...', w_trans[0:3], q_trans)) /
             np.einsum('i...,i...', w_trans[0:3], v_trans))

    # calculate points
    p_trans = q_trans + lmbda * v_trans

    # return points in expected shape
    return np.transpose(p_trans, range(1, len(p_trans.shape)) + [0])
