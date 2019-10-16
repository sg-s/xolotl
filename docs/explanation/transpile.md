This document describes how xolotl automatically generates C++ code
that sets up and integrates a model, given a model specification in
MATLAB.


## Philosophy

`xolotl` is designed to overcome two problems encountered in neuronal simulator design.

The first is the trade-off between speed and usability.
The best code is that which is easy to read and understand,
fast to write, and efficient to run.
This means that a good simulator should be easy to use, and fast to run.
We use a C++ back-end for our numerical integration schemes for this reason,
and write this code in native C++ in order to take advantage of object-oriented programming paradigms
that naturally lend themselves to modeling systems with inherent hierarchical structures.
We use a MATLAB front-end to enable use of existing tools for scientific computing,
a powerful plotting interface, and convenient features for manipulating data structures on-the-fly.

By pre-compiling a C++ function and passing parameters to it during the numerical integration,
rather than generating C++ code from a MATLAB script at compile-time,
we are able to take advantage of changing parameters swiftly between each simulation,
and extend this functionality to parallel computing.
Thus, we only need to compile for a single model structure,
and can simulate using arbitrary parameters, including while changing parameters
in real-time using sliders.

### How C++ objects are named and referenced

### How relationships between objects are inferred  

## Generating the C++ file

### How headers are resolved


### How objects are created

### How compartments are added

### How conductances are inserted into compartments

### How mechanisms are connected to objects

### How synapses are connected

## See Also


* [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#transpile)
* [xolotl.transpileCore](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#transpileCore)
* [xolotl.compile](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#compile)
