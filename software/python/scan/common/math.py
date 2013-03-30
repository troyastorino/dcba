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
    new_shape = [n_bits] + list(x.shape)

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

    return dec_array
