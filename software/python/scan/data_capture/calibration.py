import time
import cv2
import numpy as np
from optical_equipment import Camera, capture_image
import image

def generate_corner_coordinates(interior_corners, side_length):
    """
    Method: generate_corner_coordinates
    Generates a list, in board coordinates, of the
    interior corners of the checkerboard. Assumes that z=0

    Parameters:
    interior_corners - dimensions of interior corners. Can be tuple
    side_length - Length of a side of a checkerboard square

    Return
    List of OpenCV Point3f objects
    """
    x_coords = np.linspace(0, (interior_corners[0]-1)*side_length, interior_corners[0])
    y_coords = np.linspace(0, (interior_corners[1]-1)*side_length, interior_corners[1])

    points = []
    for y in y_coords:
        for x in x_coords:
            points.append([x, y, 0])

    return np.array(points)

def find_checkerboard_corners(image, interior_corners, side_length, sub_pixel = False):
    """
    Method: find_checkerboard_corners

    Parameters:
    image - ndarray of the image data
    interior_corners - dimensions of the interior corners
    side_length - length of a side of a checkerboard square

    Return: A tuple of the form (pattern_found, corners, imageData), where pattern_found is a
    boolean representing if the checkerboard pattern was found,  corners is a
    numpy array of the corner coordinates, and imageData is a numpy array of the image with
    the corners drawn on it
    """
    imageData = image.data.copy()
    
    pattern_found, corners = cv2.findChessboardCorners(imageData, interior_corners)

    if pattern_found:
        # reshpae corners because comes out of findChessboardCorners in an odd
        # fashion
        corners = np.reshape(corners, (corners.shape[0], corners.shape[2]))

        if sub_pixel:
            raise NotImplementedExcpetion();
    
        cv2.drawChessboardCorners(imageData, interior_corners, corners, pattern_found)

    return pattern_found, corners, imageData

def capture_checkerboard_calibration_images(capture, interior_corners, side_length, num_images=5, delay_time=2):
    """
    Method: capture_checkerboard_calibration_images
    Captures a sequence of images to be
    used for calibrating a camera. Shows the images being produced by the camera,
    and pauses if the image is a good one

    Parameters
    interior_corners - the dimensions of the interior corners of the checkerboard
    side_length - the side length of one of the squares of the checkerboard
    capture - OpenCV VideoCapture object that will capture the images
    num_images - the number of images desired for the capture sequence
    delay_time - the time to have the program wait between captured images to
    allow you to move the checkerboard (in seconds)

    """
    # open a window to view the images
    name = "Camera Calibration"
    cv2.namedWindow(name)

    images = []

    while len(images) < num_images:
        # collect the image
        image = capture_image(capture)

        # find the corners
        pattern_found, corners, view = find_checkerboard_corners(image, interior_corners, side_length)

        # show the image
        cv2.imshow(name, view)
        cv2.waitKey(1) # necessary for the window display

        # add the image if it has corners
        if pattern_found:
            images.append(image)

            # sleep so the user can move the checkerboard to a new angle
            time.sleep(delay_time)

    return images

def calibrate_with_checkerboard(capture, images, interior_corners, side_length):
    """
    Method calibrate_with_checkerboard:
    From a series of checkerboard images
    initializes a calibrated Camera object

    Parameters:
    capture - VideoCapture device that captured the images
    images - list of images containing the checkerboard calibration.
    This images must be from the same camera, and so must be the
    same size
    interior_corners - the dimensions of the interior corners of the
    checkerboard
    side_length - the side length of one of the squares of the
    checkerboard
    """
    # generate image and board points for all of the images
    image_points = []
    board_points = []
    board_corners = generate_corner_coordinates(interior_corners, side_length)
    for image in images:
        pattern_found, corners, _ = find_checkerboard_corners(image, interior_corners, side_length)

        if pattern_found:
            image_points.append(corners)
            board_points.append(board_corners)
        else:
            raise Exception("A checkerboard could not be found on one of the images.")

    # convert to numpy arrays of the appropriate type
    image_points = np.array(image_points, 'float32')
    board_points = np.array(board_points, 'float32')

    # find the image size
    shape = images[0].data.shape
    image_size = (shape[1], shape[0])

    # calibrate the camera
    rms, camera_matrix, distortion, rvecs, tvecs = cv2.calibrateCamera(board_points, image_points, image_size)

    import pdb; pdb.set_trace()

    # return initialized camera object
    return Camera(capture, None, camera_matrix, distortion)

if __name__ == "__main__":
    
    # initialize coordinates of the checkerboard
    interior_corners = (6,8)
    side_length = 1.8

    # choose method to get images
    capture_images = False

    if capture_images:
        # initialize the camera
        capture = cv2.VideoCapture(0)

        # capture the images
        images = capture_checkerboard_calibration_images(capture, interior_corners, side_length, num_images=1)
    else:
        # null capture
        capture = None
        
        # use test images
        img_dir = "../../../../materials/test-images/swept-plane/calib/calib/"
        images = image.load_from_directoyr(img_dir)

    # calibrate the camera
    cam = calibrate_with_checkerboard(capture, images, interior_corners, side_length)

