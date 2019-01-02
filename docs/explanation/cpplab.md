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

## How cpplab works

### How cpplab reads a C++ header file
