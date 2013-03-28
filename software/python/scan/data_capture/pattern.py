import numpy as np
from scan.common.math import gray_code, bits

class StaticPattern:
    """
    Class: StaticPattern
    A pattern class to be used for the turning on and off of static projectors,
    most likely implemented as a grid over an LED. This pattern has no
    information other than type, as static projectors can only create a single
    pattern.
    """
    pass

class DLPPattern:
    """
    Class: DLPPattern
    A pattern class to be used for the display of arbitrary patterns on DLP
    projectors. This class contains a matrix that represents the pixel values to
    be projected. This matrix is set at construction and used by DLP projectors
    to create a similar projected pattern.

    Properties:
    image - the image matrix that defines this projection
    """
    def __init__(self, image):
        self.image = image

class GeneratedPattern:
    """
    Class: GeneratedPattern
    This class represents the light field generated by a projector. It factors in
    the distortion and calibration of the projector in its image, and takes into
    account multiple projectors, if multiple projectors are used. It also
    contains a reference to the projectors that created the pattern for easy
    access later on.

    Properties:
    image - *ndarray* The projected image, in matrix form
    projectors - A list of the <Projector>s used to create the image
    """
    def __init__(self, img, projectors):
        self.image = img
        self.projectors = projectors

def gray_code_patterns(img_shape, vertical_stripes=True):
    """
    Function: gray_code_patterns
    Generates a list of <DLPPattern> objects corresponding to a sequence of
    Gray codes to be projected. Each Gray code pattern will be repeated with
    its colors inverted. The first pattern will be all white, followed by one
    that is all black, then the next pattern will have the first half be all
    white and the second half all black, followed by the inverse of that pattern

    Parameters:
    img_shape - *Tuple* of form (height, width), where width is the pixel width
    of the pattern and height is the pixel height of the pattern
    vertical_stripes - *boolean* If true, generate stripes going up and down,
    otherwise, generate horizontal stripes

    Returns:
    *[<DLPPattern>]* list of patterns for the Gray code
    """
    # see if going by width or height
    dim_idx = int(vertical_stripes)
    other_dim_idx = int(not vertical_stripes)

    # get parameters for generation
    n = np.ceil(np.log2(img_shape[dim_idx]))
    offset = np.floor((2**n - img_shape[dim_idx]) / 2)

    # list of Gray code numbers
    gray_nums = gray_code(np.arange(2**n, dtype='i'))

    # convert to bit arrays
    gray_bits = bits(gray_nums)

    # function to generate pixels from gray_code_line
    def gen_pixels(line):
        pixels = np.tile(line, ((img_shape[other_dim_idx], 1)))
        if not vertical_stripes:
            pixels = pixels.T
        return pixels

    # create the pattern sequence
    patterns = [DLPPattern(np.ones(img_shape)*255), DLPPattern(np.zeros(img_shape))]
    for i in range(gray_bits.shape[1]):
        bit_seq = gray_bits[:,i]
        patterns.append(DLPPattern(gen_pixels(bit_seq * 255)))
        patterns.append(DLPPattern(
                gen_pixels(np.logical_not(bit_seq).astype('i') * 255)))
        
    return patterns
