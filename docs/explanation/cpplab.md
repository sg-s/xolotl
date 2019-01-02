This document describes how cpplab binds C++ code to
MATLAB objects. xolotl is based on cpplab, and the xolotl
class inherits from the cpplab class. Thus, all xolotl objects
are also cpplab objects.

## The problem with existing approaches

MATLAB already has ways in which C++ code can be linked
to MATLAB and can be used within MATLAB. However, these
approaches have limitations that prevent us from using C++
code to its full potential.

### The limitations of `codegen`

The MATLAB `codegen` function (using the `Coder` toolchain) is a powerful tool for  optimizing code. It translates MATLAB code into C. Since C is a statically-typed, imperative programming language that provides constructs which map efficiently to machine instructions, it is exceptionally fast. By writing in MATLAB and allowing the `Coder` to programmatically translate that code into C, many iterated processes (such as numerical integration) can be performed much faster.

The major disadvantage with this approach is that only constructs available in both MATLAB and C may be used. Vectorized operations, booleans, dynamic typing and sizing of arrays, and `struct`s cannot be translated to C. Similarly, C constructs such as passing by reference and robust object-oriented programming cannot be exploited by the `Coder`.

This means that while simple code in MATLAB can be dramatically more efficient when translated automatically to C, for more complex computations, it is better (though much more difficult) to run models written in C or C++, using the full breadth of the language's features.

### Why not use `mex`?

## Philosophy

Xolotl solves the problem of ease-of-use vs. computational efficiency by designing the integration environment "under-the-hood" in C++ and adding a robust simulation environment in MATLAB on top. This isn't just a pretty interface -- adding a compartment in xolotl actually changes C++ code. Then the code is compiled, and when the `integrate` function is called in MATLAB, parameters are passed to the C++ function which is then run and the results returned in MATLAB.

While the C++ code can be compiled and run by itself, it's so much better with the MATLAB interface. The user never has to leave MATLAB-land. A model can be written and inspected in the command window and integrated, with top-tier C++ speed, without the user having to touch any C++ code. All the transpiling and compiling happens beneath the surface.

The main limitation of this approach is that only model components which have been pre-specified can be used. This means someone has to write out all the C++ code beforehand, so that models can be constructed from these building blocks. A lot of work has already been done to provide hundreds of network components, and several functions and templates exist to auto-generate, or at least, vastly simplify, the process of creating new components.

We hope that as our user-base grows, users will be willing to contribute their custom network components, especially ones from published papers, which will be added to the growing database of components available for any xolotl model.

## How cpplab works

### How cpplab reads a C++ header file
