import os.path

def print_obj_vars(obj):
    """
    Function: print_obj_vars
    Prints the values of all the variables in an object or dict.

    Parameters:
    obj - The object or dict to print the values of

    Returns:
    the original dict
    """
    for attr, val in vars(obj).iteritems():
        print attr, ": ", val

def file_dir(f):
    """
    Function: file_dir
    Get the directory of the given file name.  Useful if passed __file__

    Parameters:
    f - the file path to get the directory name of. Very useful if passed __file__

    Returns:
    The real directory name of the passed filename
    """
    return os.path.dirname(os.path.realpath(f))

def rel_to_file(rel_path, f):
    """
    Function: rel_to_file
    Get the absolute path of relative to a filepath.

    Parameters:
    rel_path - the relative path to be transformed into an absolute path
    f - the file path that rel_path is relative to.  Can be useful if is __file__
    """
    return os.path.join(file_dir(f), rel_path)
