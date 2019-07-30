
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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L88)

-------



### connect

**Function Signature**

```C++
void connect(compartment* comp_) 
```

**Description**


Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism under default 
conditions (single-step integration). 

void mechanism::integrate() {
    mexErrMsgTxt("[mechanism] Unimplemented integration method\n");
}





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L94)

-------



### connect

**Function Signature**

```C++
void connect(conductance* cond_) 
```

**Description**


Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism under default 
conditions (single-step integration). 

void mechanism::integrate() {
    mexErrMsgTxt("[mechanism] Unimplemented integration method\n");
}



void mechanism::connect(compartment* comp_) {
    mexErrMsgTxt("[mechanism] This mechanism cannot connect to a compartment object");
}



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L98)

-------



### connect

**Function Signature**

```C++
void connect(synapse* syn_) 
```

**Description**


Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism under default 
conditions (single-step integration). 

void mechanism::integrate() {
    mexErrMsgTxt("[mechanism] Unimplemented integration method\n");
}



void mechanism::connect(compartment* comp_) {
    mexErrMsgTxt("[mechanism] This mechanism cannot connect to a compartment object");
}

void mechanism::connect(conductance* cond_) {
    mexErrMsgTxt("[mechanism] This mechanism cannot connect to a conductance object");
}




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L103)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/mechanism.hpp#L115)

-------

