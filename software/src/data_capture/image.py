class Image:
"""
Class: Image

 Stores all the information relevant to one captured image from the
 environment. This includes the patterns projected on the object when the
 image was taken, the camera that took the image, and the image data itself,
 in matrix form.

 Properties:
 Patterns - list of <GeneratedPattern>s; light patterns projected on
 the object when the image was taken
 camera - The <Camera> that took the image
 data - ndarray The pixel data comprising the image
"""
    def __init__(self, data, camera, patterns):
        self.data = data
        self.camera = camera
        self.patterns = patterns
