dcba
=======

Repository for code and documents relating to our 6.S078 project.

[Google Drive folder](https://drive.google.com/#folders/0B_2CD27NNNbxcE1FSFVWV1JNQ0k)

Conventions
============
Programming Style
-----------------
We are writing the code in a mostly functional style. This means we should try
and treat objects as immutable, and we should write functions without
side-effects. When we get to the point of optimization, we'll break this
paradigm as needed, but we will not break it lightly.

Naming
-------
For the python source code, class names will be in `UpperCamelCase`, and
other variable names will be `underscore_separated`.  Clarity in variable names
is more important than brevity.

Image Shape
------------
In our code, the shape of image arrays is (width, height), so that pixels can be
accessed with the semantics `image[x, y]`.  

*IMPORTANT*: OpenCV uses the opposite convention for some reason. This means
 when using OpenCV functions it may be important to transpose the data. The
 functions `scan.data_capture.image.to_OpenCV` and
 `scan.data_capture.image.from_OpenCV` can be used to accomplish this transpose
 in their respective directions.
 
NaturalDocs Documenation 
========================
[NaturalDocs](http://www.naturaldocs.org/) compiles correctly formatted
documentation of the code in software into an easily readable and searchable
html page.

The only convention we have on top of those for NaturalDocs is to include the
type of a parameter or return for python documentation in bold. For example, if
we had a parameter for the pixel data of an image, it would be in this form
```
data - *ndarray* The array of pixel values for an image
```

To compile the documentation simply run make-docs.sh in ./software. Any arguments
passed to this script will be passed on to NaturalDocs. -r forces recompilation
of all documentation, and so might be helpful.

make-docs uses a script called dos2unix (can be installed with homebrew) to
convert file endings before and after documentation compilation. NaturalDocs,
otherwise, leaves CRLFs everywhere... ugh.

Compiled documentation will be placed in ./software/docs.

Dependencies
=============
The general packages you need on your system are:
* dos2unix
* python >= 2.7
* opencv >= 2.4
* pcl

If you are running OS X, it will be easiest to install these dependencies using
[Homebrew](http://mxcl.github.com/homebrew/). 

For pcl, you could either
[download a precompiled binary](http://pointclouds.org/downloads/), 
or you could install from source. Instructions for OS X with Homebrew follow. First install OpenNI: I haven't
found a good source version to use (it's patched), so
[install the precompiled version](http://sourceforge.net/projects/pointclouds/files/dependencies/OpenNI-MacOSX-v1.5.4.0.pkg/download). 
Then, add the science packages to homebrew:
```
brew tap homebrew/science
```
Next, copy the Homebrew Formula software/lib/Formula/pcl.rb to
/usr/local/Library/Formula/pcl.rb, and run
```
brew install pcl
```
That should be it!

As Python is being used for the core software library, it is recommended to use
[pip](https://pypi.python.org/pypi/pip) to install the necessary Python
packages. Install using pip by running
```
pip install <package>
```
Below are the packages that are required:
* numpy
* scipy
* nose

Unit Testing
============
We use [Nose](https://nose.readthedocs.org/en/latest/) for unit testing.  We
have a soft goal for 100\% code coverage: all code that can be tested with a
unit test should be tested, but if you think that a unit test is really
unreasonable in a certain situation, we'll trust you.  Tests should go in the
`software/python/scan/test` directory. To run all the tests, run `software/run-tests.sh`.

Looking at tests can be a good way of getting an idea of how to use the
different functions.  Additionally, if you are writing code to test how
something is working, you should be writing that code as a test!

When writing tests, in general write setup code in the setUp method for a
subclass of unittest.TestCase, but if the setup is very expensive, write it as a
class method with the setUpClass method.

If there is a very slow test or test class, mark it with the following
decorator:
```
@nose.plugins.attrib.attr('slow')
```
This way, if you want to run all tests except the slow ones, you can pass `-a
'!slow'` to the test executable.  Similarly, you could pass `-a 'slow'` to run
only the slow tests.

If you only want to run a single test module, instead of all the tests, just type in
the module name of the test after the script name, e.g.:
```
$ ./run-tests scan.tests.test_math
```
More details for what can be passed can be found in the nose documentation.  All
arguments passed to the script will be passed to nose
