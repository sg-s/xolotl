This document describes how cpplab binds C++ code to
MATLAB objects. xolotl is based on cpplab, and the xolotl
class inherits from the cpplab class. Thus, all xolotl objects
are also cpplab objects.

## The problem with existing approaches

MATLAB already has tools ([codegen](https://www.mathworks.com/help/coder/ref/codegen.htm), 
[mex](https://www.mathworks.com/help/matlab/ref/mex.html) which link C++ code to MATLAB. However, these
approaches have limitations that prevent us from using C++
code to its full potential.

### The limitations of `codegen`

The MATLAB `codegen` function automatically translates MATLAB 
code to C/C++. This translated code can be much faster. 

However, `codegen` has several limitations:

* it can only translate functions in MATLAB, and offers no support for object-oriented programming [source](https://www.mathworks.com/help/simulink/ug/how-working-with-matlab-classes-is-different-for-code-generation.html#btsyar3-1)
* `codegen` respects the primacy of MATLAB code, and therefore means that generated C++ code cannot use features in C++ that do not exist in MATLAB like pointers. 
* `codegen` is also awfully slow, and generates a lot of junk files that clutter your disk

xolotl is a component-oriented neuron and network simulator. 
As such, these "components" (i.e., compartments, synapses, conductances and mechanisms) are objects with a distinct type
that inherit from abstract classes that define them.
If we were to use `codegen`, we would have to throw away 
the rich structure of the xolotl model tree and translate
everything into a function, which would be very cumbersome. 

### Why not use `mex`?

## Philosophy

Xolotl solves the problem of ease-of-use vs. computational 
efficiency by designing the integration environment
"under-the-hood" in C++ and adding a robust simulation 
environment in MATLAB on top. This isn't just a pretty 
interface -- adding a compartment in xolotl actually 
changes C++ code. When the code is compiled, and when 
the `integrate` function is called in MATLAB, parameters 
are passed to the C++ function which is then run and the
results returned in MATLAB.

In principle, the C++ code can be compiled and run by itself. 
However, xolotl offers a very rich suite of tools to make working
with the model much easier, without ever having to leave MATLAB.
A model can be written and inspected in the command window
and integrated, with top-tier C++ speed, without the user 
having to touch any C++ code. All the transpiling and 
compiling happens silently behind the scene. 

The main limitation of this approach is that only model
components which have been pre-specified can be used. 
This means someone has to write out all the C++ code 
"beforehand", so that models can be constructed from 
these building blocks. A lot of work has already been 
done to provide hundreds of network components, and 
several functions and templates exist to auto-generate, 
or at least, vastly simplify, the process of creating
new components.

We hope that as our user-base grows, users will contribute
their custom network components, especially ones from 
published papers, which will be added to the growing 
database of components available for any xolotl model.

## How cpplab works

### How cpplab reads a C++ header file
