
This document describes the "compartment" C++ class.
This class describes objects that are compartments, and
can be used to represent neurons and parts of neurons.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| no |  conductance, mechanism, synapse | network |




## Methods



### addAxial

**Function Signature**

```C++
void addAxial(synapse *syn_) 
```

**Description**


This method adds an axial synapse to this compartment.
It updates the `n_axial_syn` property of this class
so that every compartment can keep track of how many
axial synapses are connected to it.

**See Also**

* [addSynapse](./compartment.md#addsynapse)
* [addMechanism](./compartment.md#addmechanism)
* [addConductance](./compartment.md#addconductance)





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L292)

-------



### addConductance

**Function Signature**

```C++
void addConductance(conductance *cond_) 
```

**Description**


This method adds a conductance object to this compartment.
It does the following things:

1. Adds a pointer to the conductance to a vector of pointers called `cond`
2. Updates various attributes of the conductance like verbosity, etc.
3. Runs some initializaiton code (e.g. building look-up-tables in the conductance)
4. Calls the `connect` method of that conductance with a pointer to this compartment.

**See Also**

* [addAxial](./compartment.md#addaxial)
* [addMechanism](./compartment.md#addmechanism)
* [addConductance](./compartment.md#addconductance)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L313)

-------



### addMechanism

**Function Signature**

```C++
void addMechanism(mechanism *mech_) 
```

**Description**


This method adds a mechanism object to this compartment.
It does the following things:

1. Adds a pointer to the conductance to a vector of pointers called `mech`
2. Updates various attributes of the conductance like verbosity, etc.
3. Tells the mechanism what its ordering in `mech` is by updating `mechanism_idx` in that mechanism object
4. Determines the data frame size of this object by calling `getFullStateSize` and storing this in `mechanism_sizes`

**See Also**

* [addSynapse](./compartment.md#addsynapse)
* [addConductance](./compartment.md#addconductance)





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L354)

-------



### addSynapse

**Function Signature**

```C++
void addSynapse(synapse *syn_) 
```

**Description**


This method adds a synapse to this compartment. A
pointer to this synapse is stored in the vector `syn`

**See Also**

* [addSynapse](./compartment.md#addaxial)
* [addMechanism](./compartment.md#addmechanism)
* [addConductance](./compartment.md#addconductance)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L379)

-------



### checkSolvers

**Function Signature**

```C++
void checkSolvers(int solver_order) 
```

**Description**



This method is called when a non-default solver order
is requested. The compartment checks that it can support
this solver order, and then asks every component contained
in it if they can support this solver order. This ensures
that the solver order can actually be used, because if
any component cannot support this solver order (because
those integration routines have not been written), then
they can throw an error, aborting the simulation.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L401)

-------



### getBCDF

**Function Signature**

```C++
double getBCDF(int idx)
```

**Description**


This method is a helper function used in the Crank-Nicolson
scheme and returns B, C, D and F values as defined in eq.
6.45 of "Theoretical Neuroscience" by Dayan and Abbott



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L428)

-------



### getConductancePointer

**Function Signature**

```C++
conductance* getConductancePointer(const char* cond_class) 
```

**Description**


This method returns the pointer to a conductance stored
in this compartment, identified by its class name.

!!! warning
    Note that there is another method with the same name that can be called using a integer argument.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L478)

-------



### getConductancePointer

**Function Signature**

```C++
conductance * getConductancePointer(int cond_idx)
```

**Description**


This method returns the pointer to a conductance contained
in this compartment, identified by its numeric index.

!!! warning
    Note that there is another method with the same name that can be called using a `char` argument.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L498)

-------



### getConnectedCompartment

**Function Signature**

```C++
compartment* getConnectedCompartment(int idx) 
```

**Description**


This method returns a pointer to a compartment that is linked
via axial synapses to this compartment.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L508)

-------



### getFullCurrentState

**Function Signature**

```C++
int getFullCurrentState(double *cond_state, int idx) 
```

**Description**


This method is used to read out the full dynamical state of
all channels, and to store it in a large array for output.

Since  C++ doesn't support returning multiple values, this function
works like this:

* it is first called with a pointer to the array, and an integer
specifying where it should write values to
* it writes as many values as it wants to, to the array, and returns a new integer indicating the next empty location in the array

**See Also**

* [getFullSynapseState](./compartment.md#getfullsynapsestate)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L531)

-------



### getFullMechanismSize

**Function Signature**

```C++
int getFullMechanismSize(void) 
```

**Description**


This method returns the full dimension size of all the mechanisms
in this compartment. The full size is calculated on the fly
by recursively asking all the mechanisms in this compartment
what their data dimension is, and adding up all those numbers.

**See Also**

* [getFullSynapseSize](./compartment.md#getFullSynapseSize)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L550)

-------



### getFullMechanismState

**Function Signature**

```C++
int getFullMechanismState(double *mech_state, int idx) 
```

**Description**


This method is used to read out the full dynamical state of
all mechanisms, and to store it in a large array for output.

Since  C++ doesn't support returning multiple values, this function
works like this:

* it is first called with a pointer to the array, and an integer
specifying where it should write values to
* it writes as many values as it wants to, to the array, and returns a new integer indicating the next empty location in the array

**See Also**

* [getFullSynapseState](./compartment.md#getfullsynapsestate)
* [getFullCurrentState](./compartment.md#getfullcurrentstate)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L576)

-------



### getFullSynapseSize

**Function Signature**

```C++
int getFullSynapseSize(void) 
```

**Description**


This method returns the full dimension size of all the synapses
in this compartment. The full size is calculated on the fly
by recursively asking all the synapses in this compartment
what their data dimension is, and adding up all those numbers.

**See Also**

* [getFullMechanismSize](./compartment.md#getFullMechanismSize)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L597)

-------



### getFullSynapseState

**Function Signature**

```C++
int getFullSynapseState(double *syn_state, int idx) 
```

**Description**


This method is used to read out the full dynamical state of all
synapses, and to store it in a large array for output.

Since C++ doesn't support returning multiple values, this function
works like this:

* it is first called with a pointer to the array, and an integer
specifying where it should write values to
* it writes as many values as it wants to, to the array, and returns a new integer indicating the next empty location in the array

**See Also**

* [getFullCurrentState](./compartment.md#getfullcurrentstate)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L622)

-------



### getMechanismPointer

**Function Signature**

```C++
mechanism * getMechanismPointer(int mech_idx)
```

**Description**


This method returns a pointer to a mechanism stored in this
compartment, identified by its numerical index.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L634)

-------



### getMechanismPointer

**Function Signature**

```C++
mechanism* getMechanismPointer(const char* cond_class)
```

**Description**


This method returns a pointer to a mechanism that is contained
by this compartment, that is linked to a conductance of a certain
type, also in this compartment.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L645)

-------



### getSynapsePointer

**Function Signature**

```C++
synapse * getSynapsePointer(int syn_idx)
```

**Description**


This method returns a pointer to a synapse contained by
this compartment, where this synapse is identified by the
numerical index it was added to this compartment.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L666)

-------



### integrateChannels

**Function Signature**

```C++
void integrateChannels(void) 
```

**Description**


This method integrates all the channels in the compartment. This
method is only used when this compartment is not part of a
multi-compartment model. It simply asks every conductance
object in this compartment to integrate by calling their
`integrate` methods.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L681)

-------



### integrateCNFirstPass

**Function Signature**

```C++
void integrateCNFirstPass(void) 
```

**Description**


This method is part of the Crank-Nicolson method to solve
for the voltages in a multi-compartment neuron. This is the
"first pass" down a cable. In simulations, this is called before
integrateCNSecondPass.

**See Also**

* [integrateCNSecondPass](./compartment.md#integratecnsecondpass)
* [The Crank-Nicolson Method](../../explanation/integration.md)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L740)

-------



### integrateCNSecondPass

**Function Signature**

```C++
void integrateCNSecondPass(void) 
```

**Description**


This method is part of the Crank-Nicolson method to solve
for the voltages in a multi-compartment neuron. This is the
"second pass" up a cable. In simulations, this is called after
integrateCNFirstPass.

**See Also**

* [integrateCNFirstPass](./compartment.md#integratecnFirstpass)
* [The Crank-Nicolson Method](../../explanation/integration.md)



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L789)

-------



### integrateMechanisms

**Function Signature**

```C++
void integrateMechanisms(void) 
```

**Description**


This method integrates all mechanisms in this compartment. It
simply calls the integrate method on every mechanism in the
compartment.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L808)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k)
```

**Description**


This method integrates the voltage in this compartment,
and all components contained by this compartment, using
the Runge-Kutta 4 solver. It iteratively calls the
`integrateMS` method of all components contained within
this compartment.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L821)

-------



### integrateSynapses

**Function Signature**

```C++
void integrateSynapses(void) 
```

**Description**


This method integrates all synapses in this compartment.
All that this method does is "ask" every synapse to integrate
(by calling their integrate method), and keep track of how they
contribute to `sigma_g` and `sigma_gE`.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L900)

-------



### integrateV_clamp

**Function Signature**

```C++
void integrateV_clamp(double V_clamp) 
```

**Description**


This integration method is called when a compartment is
voltage clamped. Here, the voltage is updated to the clamp
voltage, and the current required to do so is computed and
stored in `I_clamp`.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L916)

-------



### computeClampingCurrent

**Function Signature**

```C++
void computeClampingCurrent(double V_clamp) 
```

**Description**


This integration method is called when a multi-compartment
neuron model is voltage clamped. This computes the clamping
current needed for that compartment, and takes into
account current flows between compartments



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L935)

-------



### integrateVoltage

**Function Signature**

```C++
void integrateVoltage(void) 
```

**Description**


This method integrates the voltage in this compartment,
assuming this compartment is not part of a multi-compartment
neuron model, and default solver orders are being used.

When the `use_current` flag is 0, it uses the exponential
Euler method to integrate the voltage. This makes the 
explicit assumption that all conductances are linear in V
and that the currents don't need to be computed to 
integrate the voltage. 

When the `use_current` flag is 1, the currents from each 
conductance are computed, and the the voltage is integrated
using the sum total of these currents using a simple
Euler method. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L984)

-------



### resolveAxialConductances

**Function Signature**

```C++
void resolveAxialConductances(void) 
```

**Description**


This method is used to "resolve" a multi-compartment model, i.e.,
to figure out which end is which in a cable. Compartments in a multi-compartment model are linked together using `Axial` synapses
and the `tree_idx` property of the compartment is used to indicate
its distance from the soma.

This method sets the `downstream_g` and `upstream_g` properties
of this compartment based on the other compartments in the model.




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/compartment.hpp#L1031)

-------

