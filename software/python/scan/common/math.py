import numpy as np

def are_non_negative_ints(x):
    """
    Function: check_non_negative_ints
    Checks to ensure that all members of x are non_negative ints

    Parameters:
    x - *array-like* numbers to check

    Returns:
    *boolean* True if all are non_negative ints
    """
    x = np.array(x)
    return x.dtype.kind == 'u' or (x.dtype.kind == 'i' and x.min() >= 0)

def gray_code(x):
    """
    Function: gray_code
    Generates a list of Gray code numbers from a list of numbers. Numbers will
    be in the decimal form of the binary representation of the Gray code string.
    For example, the Gray code string for 2 is '11', which is 3
    in binary, so this function would have 3 in the place of 2

    Parameters:
    x - *array-like* of *int* Numbers to be converted to Gray code

    Returns:
    List of converted Gray code numbers
    """
    if not are_non_negative_ints(x):
        raise Exception('Only valid for non-negative ints')
    return np.bitwise_xor(np.right_shift(x, 1), x)

def from_gray_code(x):
    """
    Function: from_gray_code
    Converts from a list of Gray code numbers to decimal numbers. Assumes
    numbers will be in the decimal form of the binary representation of the Gray
    code string

    Parameters:
    x - *array-like* array of gray code numbers to be converted to binary numbers

    Returns:
    *ndarray* of the binary numbers from the Gray code numbers.  The return array
    will have the same shape as x
    """
    if not are_non_negative_ints(x):
        raise Exception('Only valid for non-negative ints')

    # ensure we have an ndarray
    x = np.array(x)

    # calculate number of bits in each numbers representation
    n_bits = 8 * x.dtype.itemsize

    # calculate the bit shifts required
    max_exp = int(np.ceil(np.log2(n_bits)))
    shifts = [2**i for i in range(max_exp)]
    
    # calculate the values
    for shift in shifts:
        x = x ^ (x >> shift)

    return x

def bit_array(x, n_bits=None):
    """
    Function: bit_array
    Converts numbers to their equivalent bit arrays

    Parameters:
    x - *array-like* To be converted to bit arrays
    n_bits - *int* length for each bit array.  If omitted will be
    the largest required bit size for any of the numbers

    Returns:
    *ndarray* holding the bit arrays of the passed numbers.  If x has shape s,
    then the returned array will have the same shape with an additional dimension
    on the end The additional dimension will be of length n_bits
    """
    # make sure x is a numpy array
    x = np.array(x)

    # make sure are unigned ints
    if not are_non_negative_ints(x):
        raise Exception('Only valid for non-negative ints')
    
    # find the largest number of bits for the array
    bits_required = np.ceil(np.log2(x + 1)).astype('i')
    largest_req = np.max(bits_required)
    if n_bits:
        if largest_req > n_bits:
            raise Exception("At least one number requires more bits " +
                            "than were specified")
    else:
        n_bits = largest_req

    # find the new shape of the array with the first index as the bit index
    new_shape = (n_bits,) + x.shape

    # create the new array
    bit_array = np.zeros(new_shape, dtype=np.uint8)
    x_rem = x # holder for the calculation
    for i in range(n_bits):
        place_val = int(2**(n_bits-i-1))
        bit_array[i] = x_rem / place_val
        x_rem = x_rem % place_val

    # transpose the array so the bits are the last index
    axes = range(1, len(x.shape) + 1) + [0]
    return bit_array.transpose(axes)

def pack_bit_array(x):
    """
    Function: pack_bit_array
    Converts from a bit array to decimal numbers

    Parameters:
    x - *array-like* The bit array. The last dimension of x must be the bits

    Returns:
    *ndarray* holding the array of decimal numbers.  If x has shape s,
    then the returned array will have the same shape with one less dimension
    on the end
    """
    # make sure x is a numpy array
    x = np.array(x)

    # get the shape of the array
    shape = list(x.shape)
    n_dims = len(shape)

    # get the number of bits in the array
    n_bits = shape[n_dims-1]

    # initialize the new array
    dec_array = np.zeros(shape[:n_dims-1])

    # move bits to the first index for manipulation
    axes = [n_dims-1] + range(n_dims-1)
    bit_array = np.transpose(x, axes)

    # add all the bit values to populate the decimal array
    accessor = np.ones(dec_array.shape, dtype=np.bool)
    for i in range(n_bits):
        dec_array = dec_array + bit_array[i] * 2**(n_bits-i-1)

    return dec_array.astype(np.uint64)

def normalize(x, axis=None):
    """
    Function: normalize
    Normalizes vectors along a given axis. TODO: make work across multiple axes
    (by passing a tuple to axis)

    Parameters:
    x - *array-like* Array that holds vectors to be normalized
    axis - *int* Axis that vectors to be normalizes are on. Defaults to last axis

    Returns:
    An array of normalized vectors. The normalized vectors will have norm 1, and
    the first element will be positive
    """
    # make sure a ndarray
    x = np.array(x)
    n_dim = len(x.shape)

    if axis == None:
        axis = n_dim - 1

    if axis != n_dim-1:
        raise Exception("No tests created for anything other than the last axis")

    # calculate norms
    norms = np.sqrt(np.sum(x**2, axis=axis))

    # get the sign of the first component of each vector
    axes = range(n_dim)
    axes.pop(axis)
    sign = np.sign(np.transpose(x, [axis] + axes)[0])
    if len(sign.shape) > 0:
        sign[sign == 0] = 1
    elif sign == 0:
        sign = 1

    # shape for norm and sign to successfully broadcast to x
    shape = list(x.shape)
    shape[axis] = 1

    # divide each component by its norm
    return x / np.reshape(norms, shape) * np.reshape(sign, shape)

def _fit_plane(x):
    """
    Function: _fit_plane
    Takes a list of points in 3 space and calculates the plane the most closely
    passes through them by minimizing the least square distance. Helper for fit_plane
    source: Brown byo3d fitPlane function

    Parameters:
    x - *array-like* List of points with shape (n, 3)

    Returns:
    *ndarray* Plane parameters [a, b, c, d] satisfying equation
    a*x + b*y + c*z = d
    """
    # construct linear least-squares matrix
    mean = np.mean(x, axis=0)
    U = x - mean

    # determine minimum eigenvector of U'*U
    w, v = np.linalg.eig(np.dot(U.T, U))
    min_idx = np.argmin(w)
    a, b, c = v[:, min_idx]

    # calculate d
    d = np.dot([a, b, c], mean)

    return np.array([a, b, c, d])
    
    
def fit_plane(x, axis=None):
    """
    Function: fit_plane
    Finds a best fit plane from a set of points by minimizing the least
    squares error
    TODO: make not dumb

    x - *array-like* Array of points, where the points are in 3 space
    axis - *int* The axis that the points are listed along. The means
    the next axis (with index axis+1) must have length 3. If not specified
    will be the second to last axis

    Returns:
    *ndarray* Array of plane parameters in the form [a, b, c, d], where
    a*x + b*y + c*z = d
    """
    x = np.array(x)
    n_dim = len(x.shape)

    if axis == None:
        axis = n_dim - 2
    
    if axis != n_dim - 2:
        raise Exception("No tests created for the vector list on " +
                        "anything other than the second-to-last axis. " +
                        "Wouldn't work without modification")

    if n_dim == 2:
        return _fit_plane(x)
    elif n_dim > 2:
        # get shape where the 4 vectors of coefficients will go
        shape = list(x.shape)
        shape.pop(axis)
        shape[axis] = 4
        ret = np.zeros(shape)

        # fill by recursively calling fit_plane
        for i in range(len(x)):
            ret[i] = fit_plane(x[i], axis=axis-1) # axis loses one as we make it smaller

        return ret
    else:
        raise Exception("Must be at least a 2 dimensional array")
