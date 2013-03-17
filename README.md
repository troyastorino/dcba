dcba
=======

Repository for code and documents relating to our 6.S078 project.

[Google Drive folder](https://drive.google.com/#folders/0B_2CD27NNNbxcE1FSFVWV1JNQ0k)

Compiling
==========
We're using [CMake](http://www.cmake.org/) for compiling our source code. Running
the following from the `software` directory should suffice to compile our code

```
$ cmake .
$ make
```

Dependencies
=============
Following are the packages that must be installed to run our code:
* g++
* make
* cmake
* opencv
If you are running OS X, it will be easiest to install these dependencies using [Homebrew](http://mxcl.github.com/homebrew/).

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
