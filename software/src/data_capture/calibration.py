import time
import cv2
from numpy import linspace, array
from optical_equipment import Camera, capture_image

'''
Method: generate_corner_coordinates
Generates a list, in board coordinates, of the
interior corners of the checkerboard. Assumes that z=0

Parameters:
interiorCorners - dimensions of interior corners. Can be tuple
sideLength - Length of a side of a checkerboard square

Return
List of OpenCV Point3f objects
'''
def generate_corner_coordinates(size, sideLength):
    x_coords = linspace(0, (size[0]-1)*sideLength, size[0])
    y_coords = linspace(0, (size[1]-1)*sideLength, size[1])

    points = []
    for x in x_coords:
        for y in y_coords:
            points.append([[x, y, 0]])

    return array(points)

'''
Method: find_checkerboard_corners

Parameters:
image - ndarray of the image data
interiorCornders - dimensions of the interior corners
sideLength - length of a side of a checkerboard square

Return: A tuple of the form (patternFound, corners, imageData), where patternFound is a
boolean representing if the checkerboard pattern was found,  corners is a
numpy array of the corner coordinates, and imageData is a numpy array of the image with
the corners drawn on it
'''
def find_checkerboard_corners(image, interiorCorners, sideLength, subPixel = False):
    imageData = image.data.copy()
    
    patternFound, corners = cv2.findChessboardCorners(imageData, interiorCorners)

    if subPixel:
        raise NotImplementedExcpetion();
    
    cv2.drawChessboardCorners(imageData, interiorCorners, corners, patternFound)

    return patternFound, corners, imageData

'''
Method: capture_checkerboard_calibration_images
Captures a sequence of images to be
used for calibrating a camera. Shows the images being produced by the camera,
and pauses if the image is a good one

Parameters
interiorCorners - the dimensions of the interior corners of the checkerboard
sideLength - the side length of one of the squares of the checkerboard
capture - OpenCV VideoCapture object that will capture the images
numImages - the number of images desired for the capture sequence
delayTime - the time to have the program wait between captured images to
allow you to move the checkerboard (in seconds)

'''
def capture_checkerboard_calibration_images(
    capture, interiorCorners, sideLength, numImages=5, delayTime=2):
    # open a window to view the images
    name = "Camera Calibration"
    cv2.namedWindow(name)

    images = []

    while len(images) < numImages:
        # collect the image
        image = capture_image(capture)

        # find the corners
        patternFound, corners, view = find_checkerboard_corners(image, interiorCorners, sideLength)

        # show the image
        cv2.imshow(name, view)
        cv2.waitKey(1) # necessary for the window display

        # add the image if it has corners
        if patternFound:
            images.append(image)

            # sleep so the user can move the checkerboard to a new angle
            time.sleep(delayTime)

    return images

'''
Method calibrateCameraWithCheckerboard:
From a series of checkerboard images
initializes a calibrated Camera object

Parameters:
capture - VideoCapture device that captured the images
images - list of images containing the checkerboard calibration.
This images must be from the same camera, and so must be the
same size
nteriorCorners - the dimensions of the interior corners of the
checkerboard
sideLength - the side length of one of the squares of the
checkerboard
'''
def calibrate_camera_with_checkerboard(capture, images, interiorCorners, sideLength):
    # generate image and board points for all of the images
    imagePoints = []
    boardPoints = []
    boardCorners = generate_corner_coordinates(interiorCorners, sideLength)
    for image in images:
        patternFound, corners, _ = find_checkerboard_corners(image, interiorCorners, sideLength)

        if patternFound:
            imagePoints.append(corners)
            boardPoints.append(boardCorners)
        else:
            raise Exception("A checkerboard could not be found on one of the images.")

    # convert to numpy arrays
    imagePoints = array(imagePoints)
    boardPoints = array(boardPoints)

    # find the image size
    shape = images[0].data.shape
    imageSize = (shape[0], shape[1])

    import pdb; pdb.set_trace()

    # calibrate the camera
    calibrated, cameraMatrix, distortion, rvecs, tvecs = cv2.calibrateCamera(boardPoints, imagePoints, imageSize)

    # return initialized camera object
    return Camera(capture, cameraMatrix, distortion)

if __name__ == "__main__":
    # initialize coordinates of the checkerboard
    interiorCorners = (6,8)
    sideLength = 1.8

    # initialize the camera
    capture = cv2.VideoCapture(0)

    # capture the images
    images = capture_checkerboard_calibration_images(capture, interiorCorners, sideLength, numImages=1)

    # calibrate the camera
    cam = calibrate_camera_with_checkerboard(capture, images, interiorCorners, sideLength)
