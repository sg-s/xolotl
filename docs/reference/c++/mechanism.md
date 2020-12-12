
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


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrate"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L94)

-------



### connectCompartment

**Function Signature**

```C++
void connectCompartment(compartment* comp_) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L106)

-------



### connectConductance

**Function Signature**

```C++
void connectConductance(conductance* cond_) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L120)

-------



### connectSynapse

**Function Signature**

```C++
void connectSynapse(synapse* syn_) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L134)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k, double V, double Ca) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrateMS"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L150)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L163)

-------



### getState

**Function Signature**

```C++
double getState(int i) 
```

**Description**

 This virtual method is a placeholder. If you want to read values
out of your mechanism, make sure this returns something sensible. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L171)

-------



### getFullState

**Function Signature**

```C++
int getFullState(double* cont_, int i) 
```

**Description**

 This virtual method does nothing, but can return as many dynamic
variables as you want 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L181)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L191)

-------

