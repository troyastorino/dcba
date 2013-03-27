dcba
=======

Repository for code and documents relating to our 6.S078 project.

[Google Drive folder](https://drive.google.com/#folders/0B_2CD27NNNbxcE1FSFVWV1JNQ0k)

Variable Naming Conventions
===========================
For the python source code, class names will be in UpperCamelCase, and
other variable names will be underscore_separated.  Clarity in variable names
is more important than brevity.

NaturalDocs Documenation ========================
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

If you are running OS X, it will be easiest to install these dependencies using [Homebrew](http://mxcl.github.com/homebrew/).

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

If you only want to run a single test, instead of all the tests, just type in
the module name of the test after the script name, e.g.:
```
$ ./run-tests scan.tests.test_structured_scan
```
More details for what can be passed can be found in the nose documentation.  All
arguments passed to the script will be passed to nose
