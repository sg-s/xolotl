
This document will describe how to create your own 
components in xolotl. Currently, you can create your
own conductances, mechanisms and synapses. 


## Creating new conductances 

### Create a new conductance using the [`conductance`](../../reference/conductance) class

!!! Note "Demo"
    A worked example showing how the `conductance` class can be used to generate new channels exists in `../xolotl/examples/demo_conductance`.


First, create a `conductance` object. 

```matlab
newCond = conductance;
```

Remember that you  can always see all the 
properties of the object  using `properties(newCond)`.

Then, the activation steady-state function `m_inf`, 
the inactivation steady-state function `h_inf`, 
the activation time constant function `tau_m`, and 
the inactivation time constant function `tau_h` 
must be defined, as function handles.

For example,

```matlab
newCond.m_inf = @(V,Ca) 1.0 / (1.0 + exp((V-20.0)/5.0));
% and so on
```

You must also set whether this is a Calcium conductance.

```matlab
newCond.is_Ca = false;
```

Specify the exponents of the activation 
`p` and (de)inactivation `q` gating variables
in the current equation.

```matlab
newCond.p = 4;
newCond.q = 1;
```

Finally, you must set the default reversal potential.

```matlab
newCond.default_E = -80;
```

You can also set the default activation and 
inactivation gating variable values with
`newCond.default_m` and `newCond.default_h`.

We're done! Now we can automatically generate a `C++` file:


```matlab
newCond.generateCPPFile('condName')
```

where `'condName'` is what you want to name the conductance.
A `C++` header file will be generated at `../xolotl/c++/conductances/custom/condName.hpp`.
You can now use this conductance like any other, e.g. `x.comp.add('custom/condName', 'gbar', 10)`.


### Creating a new conductance by hand

## Creating new mechanisms

## Creating new synapses 

## Where should I put them?
<a name="whereshouldIputthem"></a>

All conductances (and any other network component) are
defined by an `.hpp` header file. You can save your 
new conductance anywhere within your MATLAB path.

We organize our conductance files in the xolotl 
directory under `xolotl/c++/conductances/`. Within that 
directory, conductances are grouped by the surname of the
first author on the paper from where they were sourced. 
For example, conductances from Liu *et al.* 1998 can 
be found in `xolotl/c++/conductances/liu/`.

If an author, such as Farzan Nadim or Cristina Soto-Trevino,
happens to have written more than one paper from which we have
extracted models, the last two digits of the paper publishing year
are appended to the author name (*e.g.* `../nadim98`). 
If there are multiple papers in a single year, lowercase alphabetical indices are used (*e.g.* `../golowasch01a`).

If a paper, such as Soplata *et al.* 2017 describes 
multiple channels of a single type in different cell 
types (*e.g.* thalamocortical relay and thalamic reticular
 cells), then full-word descriptions can be used, such as
(`../soplata/thalamocortical`).
