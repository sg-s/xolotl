
This document describes the "network" C++ class.
This class describes the network object, that is used
as an entry point for all other objects.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| no |  compartment | nothing (root) |




## Methods



### checkSolvers

**Function Signature**

```C++
void checkSolvers(void) 
```

**Description**


This method verifies that all components can integrate
using the requested solver order. What this method does
is to call the `checkSolvers` in every compartment,
which in turn calls the `checkSolvers` method in every
component contained in every compartment.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L85)

-------



### resolveTree

**Function Signature**

```C++
bool resolveTree(void) 
```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model.
It does so using the `tree_idx` property in every compartment,
setting it if need be.

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise.





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L109)

-------



### addCompartment

**Function Signature**

```C++
void addCompartment(compartment *comp_) 
```

**Description**


This method adds a compartment to the network. It does the following things:
1. adds a pointer to the compartment being added to a vector called `comp`
2. Broadcasts certain global parameters like `temperature`, `dt`, etc to all compartments.
3. Updates `n_comp` to that network knows how many compartments there are.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L217)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(double * I_ext_now) 
```

**Description**


This method is used to integrate the network using a
multi-step Runge Kutta solver. This method assumes that
no compartment is being voltage clamped.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L240)

-------



### integrateMSClamp

**Function Signature**

```C++
void integrateMSClamp(double * V_clamp) 
```

**Description**


This method is used to integrate the network using a
multi-step Runge-Kutta solver. This method assumes that
one compartment is being voltage-clamped.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L262)

-------



### integrate

**Function Signature**

```C++
void integrate(double * I_ext_now) 
```

**Description**


This method is used to integrate the network using the default
single step solver. Typically, this means using the exponential-
Euler method to integrate conductances and integrate the voltages
and Calcium levels in compartments, though mechanisms can implement
their own integration schemes. Multi-compartment models are
integrated using the Crank-Nicholson scheme.

This method assumes that no compartment anywhere is being voltage clamped.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L293)

-------



### integrateClamp

**Function Signature**

```C++
void integrateClamp(double *V_clamp) 
```

**Description**


This method is used to integrate the network using the default
single step solver. Typically, this means using the exponential-
Euler method to integrate conductances and integrate the voltages
and Calcium levels in compartments, though mechanisms can implement
their own integration schemes. Multi-compartment models are
integrated using the Crank-Nicholson scheme.

This method assumes that some compartment is being voltage clamped,
and also assumes that no current is being injected into any compartment.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L390)

-------

