
This document describes the "synapse" C++ class.
This class describes objects that are synapses which are
conductances in channels whose activity depends on the
activity of another neuron.

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


This method is used to integrate the synapse and update 
its dynamical variables. Since synapses can be arbitrary dynamical
systems, this method is virtual, and needs to be overridden
by a concrete integration scheme defined in a particular 
synapse type.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L73)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k, double V, double Ca) 
```

**Description**


This method is used to integrate the synapse and update 
its dynamical variables when a multi-step solver is requestred. 
Since synapses can be arbitrary dynamical systems, this method 
is virtual, and needs to be overridden
by a concrete integration scheme defined in a particular 
synapse type.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L85)

-------



### checkSolvers

**Function Signature**

```C++
void checkSolvers(int k) 
```

**Description**


This method checks whether the synapse can use the requested
solver order to integrate its equations. If it can't, it 
should throw an error. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L95)

-------



### init

**Function Signature**

```C++
void init() 
```

**Description**


This virtual method is a placeholder initialization method. Initialization 
methods are called after the model has been constructed, and all components
have been connected, but before the model starts the simulation. This is a
good place to put code that you need to run once before the simulation. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L114)

-------



### getFullState

**Function Signature**

```C++
int getFullState(double* syn_state, int i) 
```

**Description**


This virtual method can be overridden to return the full state
of this synapse.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L121)

-------

