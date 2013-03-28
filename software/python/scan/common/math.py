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

def bits(x, nbits=None):
    """
    Function: bits
    Converts numbers to their equivalent bit arrays

    Parameters:
    x - *array-like* To be converted to bit arrays
    nbits - *int* length for each bit array.  If omitted will be
    the largest required bit size for any of the numbers

    Returns:
    *ndarray* holding the bit arrays of the passed numbers.  If x has shape s,
    then the returned array will have the same shape with an additional dimension
    on the end The additional dimension will be of length nbits
    """
    # make sure x is a numpy array
    x = np.array(x)

    # make sure are unigned ints
    if not are_non_negative_ints:
        raise Exception('Only valid for non-negative ints')
    
    # find the largest number of bits for the array
    bits_required = np.ceil(np.log2(x + 1)).astype('i')
    largest_req = np.max(bits_required)
    if nbits:
        if largest_req > nbits:
            raise Exception("At least one number requires more bits " +
                            "than were specified")
    else:
        nbits = largest_req

    # find the new shape of the array with the first index as the bit index
    dims = list(x.shape)
    dims.insert(0, nbits)
    new_shape = tuple(dims)

    # create the new array
    x_rem = x
    bit_array = np.zeros(new_shape, dtype=np.uint8)
    for i in range(nbits):
        place_val = int(2**(nbits-i-1))
        bit_array[i] = x_rem / place_val
        x_rem = x_rem % place_val

    # transpose the array so the bits are the last index
    axes = range(1, len(x.shape) + 1)
    axes.append(0)
    return bit_array.transpose(axes)
