
This document describes the "mechanism" C++ class.
This class describes objects that are mechanisms, and
can be used to represent any sort of mechanism or dynamical
system that affects compartments, conductances or synapses,
or even other mechanisms.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |




## Methods



### integrate

**Function Signature**

```C++
void integrate() 
```

**Description**


Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism under default 
conditions (single-step integration). 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L87)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k, double V, double Ca) 
```

**Description**


Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism when a multi-step
solver is requested. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L98)

-------

