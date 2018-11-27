# Creating new conductances

!!! Note "Demos"
  Code this functionality exists in `../xolotl/examples/demo_conductance`.

The `conductance` class exists to generate custom `C++` header files for novel
conductances without leaving the `MATLAB` prompt.

First, instantiate a `conductance` object. Remember that you can always see all the
properties of the object by typing its name in to the command window, or
`fieldnames(newCond)`.

```matlab
newCond = conductance;
```

Then, the activation steady-state function `m_inf`, the (de)inactivation steady-state function `h_inf`, the activation time constant function `tau_m`, and the (de)inactivation time constant function `tau_h` must be defined, as function handles.

```matlab
newCond.m_inf = @(V,Ca) 1.0 / (1.0 + exp((V-20.0)/5.0));
...
```

!!! Note "Anonymous functions in MATLAB"
  Anonymous functions in MATLAB are defined by `@` then the function arguments
  `(V, Ca)`, and then the body of the function. Anonymous functions use the current
  scope, meaning that if you have `a = 5` defined, then you can use `a` in the function
  definition.

You must also set whether this conductance fluxes Calcium.

```matlab
newCond.is_Ca = false;
```

And the exponents of the activation `p` and (de)inactivation `q` gating variables
in the current equation.

```matlab
newCond.p = 4;
newCond.q = 1;
```

Finally, you must set the default reversal potential.

```matlab
newCond.default_E = -80;
```

You can also set the default activation and inactivation gating variable values with
`newCond.default_m` and `newCond.default_h`.

To generate the `C++` header file:

```matlab
newCond.generateCPPFile('condName')
```

where `'condName'` is what you want to name the conductance.
A `C++` header file will be generated at `../xolotl/c++/conductances/custom/condName.hpp`.
You can now use this conductance like any other, e.g. `x.comp.add('custom/condName', 'gbar', 10)`.

!!! Note "Debugging `generateCPPFile`"
  If the `../xolotl/c++/` folder and its subfolders are not on your MATLAB path,
  sometimes `generateCPPFIle` will fail. You can add these folders with
  `addpath(genpath(pathToCPPFolder))` where `pathToCPPFolder` is the full file path
  to `../xolotl/c++/`.

## Creating new conductances by hand

You can create a new conductance by hand by creating a new `.hpp` file in the
[xolotl file tree](construct-models.md#whereshouldIputthem). You can find template files in
`../xolotl/c++/conductances/templates/`.

Conductances inherit from `../xolotl/c++/conductances.hpp` so if your novel conductance
follows the Hodgkin-Huxley standard form, most methods from the conductance superclass
can be used. Otherwise, you can write them yourself. Conductances can be arbitrarily
complex because they are specified in their own header files.

If you want to contribute your conductance to the xolotl project,
[send us a pull request](contributing.md).

## Where should I put them?
<a name="whereshouldIputthem"></a>

All conductances (and any other network component) are defined by
an `.hpp` header file. You can save your new conductance anywhere within your MATLAB
path.

We organize our conductance files in the xolotl directory under `xolotl/c++/conductances/`.
Within that directory, conductances are grouped by the surname of the first
author on the paper from where they were sourced. For example, conductances
from Liu *et al.* 1998 can be found in `xolotl/c++/conductances/liu/`.

If an author, such as Farzan Nadim or Cristina Soto-Trevino, happens to have
sourced papers from multiple years, the last two digits of the paper publishing year
are appended to the author name (*e.g.* `../nadim98`). If there are multiple
papers in a single year, lowercase alphabetical indices are used (*e.g.* `../golowasch01a`).

If a paper, such as Soplata *et al.* 2017 describes multiple channels of a
single type in different cell types (*e.g.* thalamocortical relay and thalamic
reticular cells), then full-word descriptions can be used, such as
(`../soplata/thalamocortical`).
