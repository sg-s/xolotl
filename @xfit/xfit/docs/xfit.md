# The xfit class

This document describes the "xfit" class.
This is a MATLAB class for parameter optimization of xolotl models.

xfit can be instantiated by running:

```matlab
p = xfit;
% you can also specify an engine
p = xfit('particleswarm');
```

## Requirements

xfit is an add-on to xolotl.
In addition to the core modules,
xfit requires

* the Parallel Computing Toolbox,
* the Optimization Toolbox,
* and the Global Optimization Toolbox.

## Properties

Every xfit object has the following properties.
To access a property, use dot notation, e.g.:

```matlab
p.x
```

You can view all the properties of an xfit object
using the built-in `properties` command:

```matlab
properties(p)
```

### `x`
This property contains a handle to the xolotl object.
Setting this property is essential to using xfit.

### `sim_func`
A function handle to the simulation function used to evaluate the model cost.
The simulation function can be any MATLAB function,
provided that the following are true:

* The first output must be the cost, which is a positive, real-valued scalar.
* The arguments must be `(x, ~, ~)`, where `x` is the xolotl object.

When xfit performs a parameter optimization routine,
it calls the `sim_func` using the xolotl object stored in the `x` property, which has been set up with trial parameters.

### `parameter_names`
This cell array of character vectors contains the names of xolotl parameters to optimize over.
The `find` method of xolotl is the best way to populate this list.
`seed`, `lb`, and `ub` share one-to-one correspondence with `parameter_names`, so all should be the same dimensions.

### `seed`
The seed is an $n$ x 1 vector of numerical parameter values
for starting an optimization protocol,
where $n$ is the number of parameters to optimize over.

### `lb` & `ub`
`lb` and `ub` are $n$ x 1 vector of numerical lower bound and upper bound values.
During optimization, parameters are bounded between their upper and lower bounds.

### `options`
This property is a struct that holds options for the selected optimization engine.
It is automatically generated from MATLAB's built-in [optimoptions](https://www.mathworks.com/help/optim/ug/optimization-options-reference.html) function.

### `display_type`
This option determines the display type. It defaults to 'iter'.

### `engine`
This option determines the optimization algorithm used.

| Engine Name | Engine Keyword |
| ----------- | -------------- |
| Pattern Search | `'patternsearch'` |
| Particle Swarm | `'particleswarm'` |
| Genetic Algorithm | `'ga'` |

### `timestamp`
This property keeps track of the duration of a simulation.

### `best_cost`
The best cost holds the lowest value computed by the simulation function during an optimization procedure.

### `data`
The `data` property can hold any user-defined data.
