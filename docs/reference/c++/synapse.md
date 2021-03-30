
This document describes the "synapse" C++ class.
This class describes objects that are synapses which are
conductances in channels whose activity depends on the
activity of another neuron.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |




## Methods



### connect

**Function Signature**

```C++
void connect(compartment *pcomp1_, compartment *pcomp2_) 
```

**Description**


This method connects two compartments using this synapse. 
It also triggers the post-synaptic compartment to "add"
this synapse to the post-synaptic compartment, so that its
integration can be handled there.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L20)

-------



### getFullState

**Function Signature**

```C++
int getFullState(double* syn_state, int idx) 
```

**Description**


This virtual method can be overridden to return the full state
of this synapse.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L33)

-------



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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/synapse.hpp#L80)

-------



### getCurrent

**Function Signature**

```C++
double getCurrent(double V_post) 
```

**Description**



This method returns the current that flows through
this synapse at this moment. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/synapse.hpp#L93)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/synapse.hpp#L104)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/synapse.hpp#L114)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/synapse.hpp#L133)

-------

