from image import Image
import numpy as np

class OpticalEquipment(object):
    """
    Class: OpticalEquipment
    Generic superclass for all cameras and projectors used in the set up. Cameras
    and projectors can both be represented as an imaging surface and a reference
    point. This class contains the information defining those two features and
    their calibration.

    Superclass of <Projector> and <Camera>

    Attributes:
    R - *ndarray* The 3x3 rotation matrix rotating from the global coordinate frame to the
    coordinate frame of the piece of optical equipment
    T - *ndarray* The length 3 numpy array giving the position of the piece of optical
    equipment in the global coordinate frame
    extrinsic_matrix - *ndarray* The 4x3 matrix representing the position and
    orientation of the piece of optical equipment in world coordinates. Is equivalent to [R|T]
    focal_length - *ndarray* 2x1 vector representing the x and y focal lengths for the
    camera, i.e. [fx, fy]
    principal_point - *ndarray* 2x1 vector representing the two components of the principal
    point, or camera center. In the form [cx, cy]
    alpha - The skew, i.e., the angle that the x and y pixel axes are off by, e.g.,
    alpha of .5 would imply the angle between the x and y pixel axes is 89.5
    intrinsic_matrix - *ndarray* The 3x3 matrix representing the intrinsic parameters of
    the piece of optical equipment. In the form:
    [[fx, alpha*fx, cx],
     [0, fy, cy],
     [0, 0, 1]]
    distortion - *ndarray* The lens distortion coefficients of the optical equipment
    """ 
    def __init__(self, extrinsic_matrix, intrinsic_matrix, distortion):
        self.extrinsic_matrix = np.array(extrinsic_matrix)
        (self.R, self.T) = self.extrinsic_parameters(self.extrinsic_matrix)
        self.intrinsic_matrix = np.array(intrinsic_matrix)
        (self.focal_length, self.principal_point, self.alpha) = self.intrinsic_parameters(self.intrinsic_matrix)
        self.distortion = np.array(distortion)

    @staticmethod
    def intrinsic_matrix(focal_length, principal_point, alpha=0):
        """
        Function: intrinsic_matrix
        Generates the intrinsic matrix from intrinsic parameters. See the <OpticalEquipment>
        class attributes for information on the parameters

        Returns:
        Intrinsic matrix from the passed parameters
        """
        return np.array([[focal_length[0], alpha, principal_point[0]],
                         [0, focal_length[1], principal_point[1]],
                         [0, 0, 1]])

    @staticmethod
    def intrinsic_parameters(intrinsic_matrix):
        """
        Function: intrinsic_parameters
        Extracts the intrinsic parameters from an intrinsic matrix. See the <OpticalEquipment>
        class attributes for information on the parameters

        Parameters:
        intrinsic_matrix - *ndarray* 3x3 matrix of camera's intrinsic parameters

        Returns:
        Tuple of intrinsic parameters
        (focal_length, principal_point, alpha)
        """
        focal_length = np.array([intrinsic_matrix[0,0], intrinsic_matrix[1,1]])
        principal_point = np.array([intrinsic_matrix[0,2], intrinsic_matrix[1,2]])
        alpha = intrinsic_matrix[0,1]

        return (focal_length, principal_point, alpha)

    @staticmethod
    def extrinsic_matrix(R, T):
        """
        Function: extrinsic_matrix
        Calculates the 3x4 matrix representing the position and orientation of the piece of optical
        equipment in space.  Is a matrix of the form [R|T].  See the attributes of the <OpticalEquipment>
        for the parameter definitions

        Returns:
        *ndarray* with shape (4,3)
        """
        return np.concatenate((R, np.array([T]).T), axis=1)

    @staticmethod
    def extrinsic_parameters(extrinsic_matrix):
        """
        Function: extrinsic_parameters
        Returns a tuple of the rotation matrix and the translation vector giving the orientation and
        position of the piece of optical equipment.

        Parameters:
        extrinsic_martix - *ndarray* with shape (3,4) array in the form [R|T]

        Returns:
        Tuple (R, T), where R is the *ndarray* with shape (3,3) representing the rotation matrix,
        and T is the *ndarray* with shape (3,) representing the translation vector
        """
        R = extrinsic_matrix[:,0:3]
        T = extrinsic_matrix[:,3]
        return (R, T)

    def undistort(self, image):
        """
        Method: undistort
        Removes the distortion from an image using the parameters of the <OpticalEquipment>

        Parameters:
        image - *<Image>* The image to be undistorted

        Returns:
        An <Image> object with the image data undistorted
        """
        if image.corrected:
            return image
        else:
            corrected_data = cv2.undistort(image.data, self.intrinsic_matrix, self.distortion)
            return Image(corrected_data, image.camera, image.patterns, True)
    
class Camera(OpticalEquipment):
    """
    Class: Camera
    Contains all information relevant to a physical camera. This
    includes a reference to the video capture device itself and the information
    necessary to convert images from the world space to camera space that result
    from calibration.

    Subclass of <OpticalEquipment>

    Attributes:
    device - *OpenCV VideoCapture* object that can be used to capture an image
    """
    def __init__(self, device, extrinsic_matrix, intrinsic_matrix, distortion):
        super(Camera, self).__init__(extrinsic_matrix, intrinsic_matrix, distortion)
        self.device = device

    def capture_image(self):
        captured_image, image = self.device.read()
        if captured_image:
            return Image(image, self, None, False)
        else:
            raise Exception("Image capture failed.")
        
def capture_image(device):
    """
    Function: capture_image
    Given the device location information for a camera, this orders that
    camera to take a picture, and then captures the information in that
    picture and returns it as an <Image> object

    Parameters:
    device - OpenCV VideoCapture object to take the image with

    Return:
    An <Image> object containing the captured data
    """
    camera = Camera(device, None, None, None)
    return camera.capture_image()


class Projector(OpticalEquipment):
    """
    Class: Projector
    Contains all information relevant to a physical projector, whether it is a
    DLP, LED-and-grating, or any other kind of system. This system type is
    defined by the <type> property and is used to make sure a given projector can
    project a given pattern in <projectPattern>.

    Subclass of <OpticalEquipment>
    Superclass of <DLPProjector>
    """        
    pass



class DLPProjector(Projector):
    def project_pattern(self, pattern):
        """
        Method: project_pattern
        Takes a <DLPPattern>, and projects the given
        pattern with the given projector. It returns a <GeneratedPattern>
        object representing the actual light field created by the projection.
        
        Simultaneously, it projects this pattern in the physical world using the
        provided projector.
        
        Parameters:
        pattern - the <DLPPattern> to be created by the projector

        Return:
        a generated pattern that (nearly) perfectly represents the light field
        created by the projector.
        """
        pass
