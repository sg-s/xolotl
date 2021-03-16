
This document describes the "mechanism" C++ class.
This class describes objects that are mechanisms, and
can be used to represent any sort of mechanism or dynamical
system that affects compartments, conductances or synapses,
or even other mechanisms.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |




## Methods



### getPrevState

**Function Signature**

```C++
double getPrevState(int i) 
```

**Description**


This method returns the previous state of the value stored in this 
mechanism by pulling it out of the mech_states array stored in the compartment that this mechanism belongs to 

**See Also**

* [getState](./mechanism.m#getstate)
* [getFullState](./mechanism.m#getfullstate)





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L353)

-------



### getFullState

**Function Signature**

```C++
int getFullState(double* A, int idx) 
```

**Description**


THis method reads out the full state from the mechanism 
and writes it to a provided array, returning the index of 
the next position to write to.

This method is used in two scenarios:

1. In reading out mechanism state to output to the user
2. In reading out mechanism state and storing it in the compartment's mech_state property to allow for synchronous updates. 


**See Also**

* [getState](./mechanism.m#getstate)
* [getPrevState](./mechanism.m#getprevstate)






 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L379)

-------



### findMechanismNamedControlling

**Function Signature**

```C++
mechanism* findMechanismNamedControlling(std::string get_this_name, std::string should_control) 
```

**Description**


This helper function find ONE mechanism with a certain name, 
that also control a certain thing, in
the current compartment and returns a pointer to it.


**See Also**

* [findMechanismsOfTypeControlling](./mechanism.m#findmechanismsoftypecontrolling)
* [findMechanismsOfType](./mechanism.m#findmechanismsoftype)





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L403)

-------



### findMechanismsOfTypeControlling

**Function Signature**

```C++
std::vector<mechanism*> findMechanismsOfTypeControlling(std::string get_this_type, std::string should_control) 
```

**Description**


This helper function find all mechanisms of a requested type, 
that also control a requested class of mechanism, in
the current compartment and returns a vector of pointers to them.
You are free to do whatever you like with that vector. A typical
use case would be to find mechanisms of a required type, then connect to them from another mechanism. 

**See Also**

* [findMechanismNamedControlling](./mechanism.m#findmechanismnamedcontrolling)
* [findMechanismsOfType](./mechanism.m#findmechanismsoftype)





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L461)

-------



### findMechanismsOfType

**Function Signature**

```C++
std::vector<mechanism*> findMechanismsOfType(std::string get_this_type) 
```

**Description**


This helper function find all mechanisms of a requested type in
the current compartment and returns a vector of pointers to them.
You are free to do whatever you like with that vector. A typical
use case would be to find mechanisms of a required type, then connect to them from another mechanism. 

**See Also**

* [findMechanismNamedControlling](./mechanism.m#findmechanismnamedcontrolling)
* [findMechanismsOfTypeControlling](./mechanism.m#findmechanismsoftypecontrolling)






 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L519)

-------



### findMechanismsControlling

**Function Signature**

```C++
std::vector<mechanism*> findMechanismsControlling(std::string should_control) 
```

**Description**


This helper function find all mechanisms that control a specified
class of mechanisms  in
the current compartment and returns a vector of pointers to them.
You are free to do whatever you like with that vector. A typical
use case would be to find mechanisms of a required type, then connect to them from another mechanism. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L568)

-------



### connectCompartment

**Function Signature**

```C++
void connectCompartment(compartment* comp_) 
```

**Description**


This virtual method is a placeholder method of mechanism connects 
a mechanism to a compartment. It sets the "comp" pointer 
in the mechanism, and tells the parent compartment about this 
mechanism (via addMechanism). 

If you want to prevent your mechanism from connecting to a 
compartment, override this method with something that 
throws a runtime error. 

**See Also**

* [connectConductance](./mechanism.m#connectconductance)
* [connectSynapse](./mechanism.m#connectsynapse)





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L628)

-------



### connectConductance

**Function Signature**

```C++
void connectConductance(conductance* channel_) 
```

**Description**


This virtual method is a placeholder method of mechanism connects 
a mechanism to a conductance. It sets the "channel" pointer 
in the mechanism, and tells the parent compartment about this 
mechanism (via addMechanism). 

If you want to prevent your mechanism from connecting to a 
conductance, override this method with something that 
throws a runtime error. 


**See Also**

* [connectCompartment](./mechanism.m#connectcompartment)
* [connectSynapse](./mechanism.m#connectsynapse)






 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L656)

-------



### connectSynapse

**Function Signature**

```C++
void connectSynapse(synapse* syn_) 
```

**Description**


This virtual method is a placeholder method of mechanism connects 
a mechanism to a conductance. It sets the "channel" pointer 
in the mechanism, and tells the parent compartment about this 
mechanism (via addMechanism). 

If you want to prevent your mechanism from connecting to a 
conductance, override this method with something that 
throws a runtime error. 

**See Also**

* [connectCompartment](./mechanism.m#connectcompartment)
* [connectConductance](./mechanism.m#connectconductance)






 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/extra_methods.hpp#L685)

-------



### integrate

**Function Signature**

```C++
void integrate() 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing. If your mechanism is properly 
written, this will not be run because your mechanism will define a "integrate"
method, which will be used instead of this.

It's perfectly OK to have a mechanism that does nothing. For example, such a mechanism would essentially be a struct storing some value. There is no performance penalty for such mechanisms.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/mechanism.hpp#L124)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k, double V, double Ca) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing. If your mechanism is properly 
written, this will not be run because your mechanism will define a "integrateMS"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/mechanism.hpp#L136)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/mechanism.hpp#L149)

-------



### getState

**Function Signature**

```C++
double getState(int i) 
```

**Description**

 The getState method is a very important method of mechanism
and is used for two primary purposes:

1. To pass information from one mechanism to another
2. To read out mechanism states and output time-series 

Every mechanism can define as many "states" as it wants. Make sure 
that the number of states (defined in the "fullStateSize" property)
matches the actual number of states. xolotl at some point will 
call your mechanism with various integers up to fullStateSize-1
so make sure you return something sensible. 

The typical way to write a getState method is to use a switch 
statement, because the argument is an integer, and because 
switches are fast. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/mechanism.hpp#L171)

-------



### checkSolvers

**Function Signature**

```C++
void checkSolvers(int k) 
```

**Description**

 This virtual method only allows runs if solver_order is 0
If your mechanism supports other solver orders, say so in your method
to override this. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c++/mechanism.hpp#L182)

-------

