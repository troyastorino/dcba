from image import Image

class OpticalEquipment(object):
    """
    Class: OpticalEquipment
    Generic superclass for all cameras and projectors used in the set up. Cameras
    and projectors can both be represented as an imaging surface and a reference
    point. This class contains the information defining those two features and
    their calibration.

    Superclass of <Projector> and <Camera>

    Attributes:
    pose - ndarray The 4x4 matrix representing the position and
    orientation of the piece of optical equipment in world coordinates
    intrinsicMatrix - ndarray The 3x3 matrix representing the intrinsic parameters of
    the piece of optical equipment
    distortion - ndarray The lens distortion coefficients of the optical equipment
    """ 
    def __init__(self, pose, intrinsicMatrix, distortion):
        self.pose = pose
        self.intrinsicMatrix = intrinsicMatrix
        self.distortion = distortion

class Camera(OpticalEquipment):
    """
    Class: Camera
    Contains all information relevant to a physical camera. This
    includes a reference to the video capture device itself and the information
    necessary to convert images from the world space to camera space that result
    from calibration.

    Subclass of <OpticalEquipment>

    Attributes:
    device - OpenCV VideoCapture object that can be used to capture an image
    """
    def __init__(self, device, pose, intrinsicMatrix, distortion):
        super(Camera, self).__init__(pose, intrinsicMatrix, distortion)
        self.device = device

    def capture_image(self):
        capturedImage, image = self.device.read()
        if capturedImage:
            return Image(image, self, None)
        else:
            raise Excpetion("Image capture failed. retval=" + retval)
        
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
