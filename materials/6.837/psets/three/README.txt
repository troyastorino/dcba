Homework 3: Particle Animations

Compiling:

This code can be compiled easily on linux systems. cd into the directory containing the header and c++ code files, and run the command "make". This will compile all necessary files.

Running:

A new executable, "a3", will have been created by the compilation step above. To run it cd into the directory containing this new executable, and run the command "./a3 <integrator> [<step-size>]", where you can substitute "t", "e", or "r" for "<integrator>". "t" will instruct the code to use the trapazoidal integration strategy, "e" the explicit euler, and "r" the RK4 integrator. The optional parameter "<step-size>" should either be left off or replaced with a decimal number. This number will be the step size used by the integrators. If left off, a step size of 0.05 will be used.

Once the program has been started, pressing "t" toggles among the simple system, the 4-particle pendulum system, and the cloth system. Pressing "b" when viewing the cloth blows a simulated "breeze" through the cloth, giving all vertices a random additional velocity in the z direction.

Collaboration:

None.

Resources:

I used cplusplus.com extensively for syntax and library information. stackoverflow.com answered many of my other questions about syntax and compilation errors. http://www.unknownroad.com/rtfm/gdbtut/gdbsegfault.html helped me figure out how to debug segfaults, although I didn't end up using their advice all too much. The c++ review slides from the beginning of the year as well as the slides used int he particle animation lectures came in useful, too.

Problems:

There are no known problems with my code.

Extra Credit:

I did not do any of the extra credit.

Comments:

Again, loved the assignment, but I have no particularly constructive (nor deconstructive, for that matter) comments.
