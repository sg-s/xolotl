
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
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrate"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L90)

-------



### connect

**Function Signature**

```C++
void connect(compartment* comp_) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L102)

-------



### connect

**Function Signature**

```C++
void connect(conductance* cond_) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L113)

-------



### connect

**Function Signature**

```C++
void connect(synapse* syn_) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L124)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k, double V, double Ca) 
```

**Description**


This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrateMS"
method, which will be used instead of this.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L137)

-------

