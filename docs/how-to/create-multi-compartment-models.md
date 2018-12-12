This document describes how to create and work with multi compartment models in xolotl.

# Introduction and Limitations

xolotl is limited in how it can work with multi-compartment models:

1. The only type of multi-compartment models fully supported are linear cables, with the soma at one end and the neurite coming out of it.
2. You can in principle create arbitrary topologies with branching, but you are limited in the integration methods you can use (you can only use Exponential Euler, with no support for implicit solvers or multi-step solvers).
3. For linear cables, xolotl can use an implicit Crank-Nicholson scheme to solve for the voltages in different parts of the cable. This is efficient and more correct, and requires you to connect parts of the cable together using the `Axial` synapse.
4. You cannot use the Runge-Kutta 4 solver with multi-compartment models.

With this out of the way, let's understand how multi-compartment models can be specified in xolotl.

# Making a cable using the `Axial` synapse type

The `Axial` synapse is a special type of electrical synapse that forces you to think of compartments as cylinders, that have a radius and a length. When two compartments are connected using the a Axial synapse, they are assumed to be part of a cable, and xolotl will attempt to use the Crank-Nicholson scheme to solve the voltage in all compartments on the cable implicity.

Thus, the general recipe for creating a multi-compartment cable is:

```matlab
x.add('compartment','Soma')
x.add('compartment','Neurite')
x.slice('Neurite',10)
x.connect('Neurite01','Soma')
```

Note that we're using the `slice` function to slice up a cylinder into shorter sections, which also automatically wires them up using `Axial` synapses.

An example showing a mulit-compartment model is found in `demo_multi_compartment.m`
