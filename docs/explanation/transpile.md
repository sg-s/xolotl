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

Each `cpplab` object has a few properties that describe what it is.
For example, the `cpp_class_name` property describes what C++ class the MATLAB object represents,
and the `cpp_class_path` property indicates where the originating header file is saved.

Consider a model of a bursting neuron:

```
>> x = xolotl.examples.BurstingNeuron;
>> x.AB.cpp_class_name

ans =

    'compartment'

>> x.AB.cpp_class_path

ans =

    '/home/alec/code/xolotl/c++/compartment.hpp'
```

All `cpplab` objects have these properties.
We can drill further into the `xolotl` object, looking at a conductance:

```
>> x.AB.NaV.cpp_class_name

ans =

    'NaV'

>> x.AB.NaV.cpp_class_path

ans =

    '/home/alec/code/xolotl/c++/conductances/prinz/NaV.hpp'
```

`cpplab` objects which are instantiated from a class which inherits from another
keep track of which class was their "parent".
For example, all objects with the class name `'NaV'` have the parent class `'conductance'`.

### How relationships between objects are inferred  

At the top of every `xolotl` model is the `xolotl` object.
When a new object is added or instantiated and added using the `add` function,
it is affixed to the model structure.
The parent object keeps track of all tunable parameters in the `cpp_lab_real_names` property.
This list can grow quite large.
For example, the 3-compartment model of the pyloric network included with `xolotl`
contains 155 parameters.

On the back-end, each C++ object uses pointers to the parent and child objects to keep track of relations.
For example, the `conductance` class comes with a `container` property
that points to which compartment "owns" it.
Accordingly, the `compartment` class comes with a vector of pointers
that keep track of all conductances belonging to that compartment.

## Generating the C++ file

When the model structure changes, the C++ file is transpiled and compiled.
Transpiling is the process of going from the extant MATLAB `xolotl` object
to a C++ source code file, which can then be compiled.

Transpiling works by copying and editing a template file.
This assures that everything is added in the correct places.
Once the header files are included in the correct order,
the `mexFunction` is put together.
This function expects a vector of numerical parameters as input,
and performs the integration, returning the desired outputs.
All parameters are declared,
then the constructors are called to instantiate the objects.

Next, network components are wired together via their properties containing pointers,
and a list of synapses is created.


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
