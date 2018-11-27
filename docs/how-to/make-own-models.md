In this document we will learn how to create novel components that do not exist in xolotl

## Creating new conductances using the `conductance` class

> code testing this functionality exists in `../xolotl/examples/demo_conductance`.

The `conductance` class exists to generate custom `C++` header files for novel
conductances without leaving the `MATLAB` prompt.

First, instantiate a `conductance` object. Remember that you can always see all the
properties of the object by typing its name in to the command window, or
`fieldnames(temp)`.

```matlab
temp = conductance;
```

Then, the activation steady-state function `m_inf`, the (de)inactivation steady-state function `h_inf`, the activation time constant function `tau_m`, and the (de)inactivation time constant function `tau_h` must be defined, as function handles.

```matlab
temp.m_inf = @(V,Ca) 1.0 / (1.0 + exp((V-20.0)/5.0));
...
```

!!! Note "Anonymous functions in MATLAB"
  Anonymous functions in MATLAB are defined by `@` then the function arguments
  `(V, Ca)`, and then the body of the function. Anonymous functions use the current
  scope, meaning that if you have `a = 5` defined, then you can use `a` in the function
  definition.

You must also set whether this conductance fluxes Calcium.

```matlab
temp.is_Ca = false;
```

And the exponents of the activation `p` and (de)inactivation `q` gating variables
in the current equation.

```matlab
temp.p = 4;
temp.q = 1;
```

Finally, you must set the default reversal potential.

```matlab
temp.default_E = -80;
```

You can also set the default activation and inactivation gating variable values with
`temp.default_m` and `temp.default_h`.

To generate the `C++` header file:

```matlab
temp.generateCPPFile('condName')
```

A `C++` header file will be generated at `../xolotl/c++/conductances/custom/condName.hpp`.
You can now use this conductance like any other, e.g. `x.comp.add('custom/condName', 'gbar', 10)`.


## Creating new conductances by hand

## Creating new mechanisms
