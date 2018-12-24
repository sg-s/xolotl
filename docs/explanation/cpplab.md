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


### Why not use `mex`? 

## Philosophy 

## How cpplab works 

### How cpplab reads a C++ header file