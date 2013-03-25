dcba
=======

Repository for code and documents relating to our 6.S078 project.

[Google Drive folder](https://drive.google.com/#folders/0B_2CD27NNNbxcE1FSFVWV1JNQ0k)


Dependencies
=============
The general packages you need on your system are:
* dos2unix
* opencv

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

NaturalDocs Documenation
========================

[NaturalDocs](http://www.naturaldocs.org/) compiles correctly formatted documentation of the code in
software/src and software/include into an easily readable and searchable html
page.

To compile the documentation simply run make-docs in ./software. Any arguments
passed to this script will be passed on to NaturalDocs. -r forces recompilation
of all documentation, and so might be helpful.

make-docs uses a script called dos2unix (can be installed with homebrew) to
convert file endings before and after documentation compilation. NaturalDocs,
otherwise, leaves CRLFs everywhere... ugh.

Compiled documentation will be placed in ./software/docs.
