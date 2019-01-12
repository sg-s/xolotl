
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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L69)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L81)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/synapse.hpp#L91)

-------

