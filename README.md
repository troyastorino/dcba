dcba
=======

Repository for code and documents relating to our 6.S078 project.

[Areas of exploration google doc](https://docs.google.com/document/d/1VfANuXEYYeTAmdtjizWe-wBU-ze-UnLU641EQJXA3GM/edit)

NaturalDocs Documenation
========================

NaturalDocs (http://www.naturaldocs.org/) compiles correctly formatted documentation of the code in
software/src and software/include into an easily readable and searchable html
page.

To compile the documentation simply run make-docs in ./software. Any arguments
passed to this script will be passed on to NaturalDocs. -r forces recompilation
of all documentation, and so might be helpful.

make-docs uses a script called dos2unix (can be installed with homebrew) to
convert file endings before and after documentation compilation. NaturalDocs,
otherwise, leaves CRLFs everywhere... ugh.

Compiled documentation will be placed in ./software/docs.
