This document describes how to create and work with multi compartment models in xolotl.

## Introduction and Limitations

xolotl is limited in how it can work with multi-compartment models:

1. The only type of multi-compartment models fully supported are linear cables, with the soma at one end and the neurite coming out of it.
2. You can in principle create arbitrary topologies with branching, but you are limited in the integration methods you can use (you can only use Exponential Euler, with no support for implicit solvers or multi-step solvers).
3. For linear cables, xolotl can use an implicit Crank-Nicholson scheme to solve for the voltages in different parts of the cable. This is efficient and more correct, and requires you to connect parts of the cable together using the `Axial` synapse.
4. You cannot use the Runge-Kutta 4 solver with multi-compartment models.

With this out of the way, let's understand how multi-compartment models can be specified in xolotl.

## Making a cable using the `Axial` synapse type

The `Axial` synapse is a special type of electrical synapse that forces you to think of compartments as cylinders, that have a radius and a length. When two compartments are connected using the a Axial synapse, they are assumed to be part of a cable, and xolotl will attempt to use the Crank-Nicholson scheme to solve for the voltage in all compartments on the cable implicitly.

Thus, the general recipe for creating a multi-compartment cable is:

```matlab
x.add('compartment','Soma')
x.add('compartment','Neurite')
x.slice('Neurite',10)
x.connect('Neurite01','Soma')
x.Soma.tree_idx = 0; % mark this as the cell body
```


!!! warning 
    `tree_idx`, and the presence of `Axial` synapses, is how xolotl "knows" a model is a multi-compartment model vs a set of a individual neurons that are electrically coupled. You must specify some compartment as the cell body using `tree_idx`, otherwise, xolotl will not consider it a multi-compartment model. 

Note that we're using the `slice` function to slice up a cylinder into shorter sections, which also automatically wires them up using `Axial` synapses.

An example showing a multi-compartment model is found in `demo_multi_compartment.m`

## Maximal conductance of `Axial` synapses

The maximal conductance of an `Axial` synapse from the presynaptic compartment to the postsynaptic compartment is computed using Equation 6.30 from Dayan & Abbott (2001).

$$g_{max} = \frac{r_{pre} r_{post}^2}{\rho L_{pre} (L_{pre} r_{post}^2 + L_{post} r_pre}^2)$$

where $r$ is the axial radius and $L$ the length. $\rho$ is the axial resistivity in the
presynaptic compartment. Subscripts "pre" and "post" refer to the presynaptic and postsynaptic compartments.


## Size and shape of compartments

In single-compartment models, or networks of these, it is acceptable to use the `A` and `V`
properties of compartments to specify the surface area and volume respectively.

In multi-compartment models in which the compartments are connected by `Axial` synapses, the
`len` and `radius` properties, for the length and axial radius must be specified instead.
The shape of the compartment is cylindrical, and the surface area is computed automatically,
according to the "body" of the cylinder (i.e. $2 \pi r L$).

While the `A` and `V` compartment properties aren't used in this case, you can still set them to values.
If you want them to automatically update, you can set them to anonymous functions:

```matlab
x.comp.A = @() x.comp.len * 2 * pi * x.comp.radius;
```
