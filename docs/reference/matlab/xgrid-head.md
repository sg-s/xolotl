# The xgrid class

This document describes the "xgrid" class.
This is a MATLAB class that parallelizes xolotl simulations over one or more computing clusters.
It requires the [Parallel Computing Toolbox](https://www.mathworks.com/products/parallel-computing.html) for MATLAB.

To instantiate an xgrid object, use

```matlab
p = xgrid
```

which defaults to the particle swarm optimization engine,
or specify an engine like so:

```matlab
p = xgrid('engine')
```

## Properties

Every xgrid object has the following properties.
To access any property, use dot notation, e.g.:

```matlab
p.verbosity
```

### `x`

This property contains the xolotl object that comprises the model to be simulated.
The structure of the model is important, but the exact parameter values do not,
since xgrid will spawn multiple copies of the xolotl object
and set parameter values during the parallelized simulations.

### `sim_func`

This property is a function handle to the MATLAB function is used to simulate the xolotl models.
The function must have at least one output and have the function signature

```matlab
function [outputs] = sim_func(x)
```

where `x` is the xolotl object.

There can be any number of outputs. xgrid automatically captures all outputs and saves them. 

### `n_batches`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| 10      | 1, 2, 3, ...   | double |

The user can choose how many batches of simulations each worker will perform.
A batch is a set of simulations that a worker will perform
before checking to see if there is another.

### `verbosity`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| 1      | 0, 1   | double |

Determines how much informative text xgrid will print to the command window.
When `p.verbosity == 0`, xgrid will be as quiet as possible.

### `clusters`

An internal property that keeps track of the computing clusters recruited to run xolotl simulations.

### `stagger_time`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| 1      | 1+  | double |

The amount of time (in seconds) between attempting to start a new batch of simulations.

!!! warning
    It is not advisable to reduce the stagger time below 1 second.

### `num_workers`

An internal property that keeps track of the number of workers recruited.

### `n_outputs`

An internal property that keeps track of how many outputs of the simulation function there are.

### `workers`

A protected property that lists the workers recruited.

### `n_sims`
A protected property that keeps track of how many simulations should be performed.
This is computed from the dimensionality of the input arguments to `p.batchify`.

### `xolotl_hash`

A protected property that keeps track of the MD5 hash of the xolotl object.
The hash does not change when parameters are changed,
only when the structure of the model changes
(viz. when `p.x` is modified).

### `current_pool`

A protected property for accessing the current parallel pool.

### `daemon_handle`

A protected property that consists of a vector of timers,
each of which handles a daemon on a remote cluster.

### `is_master`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| false      | false, true  | logical |

A protected flag that keeps track of whether this computer is the controlling computer.

### `speed`

A protected property listing the speed of simulation
as a ratio of time lapsed in the simulated world divided by the time lapsed in the real world.

### `xgrid_folder`

A hidden and protected property that lists the main directory of xgrid.

### `sim_start_time`

A hidden and protected property that keeps track of when simulations started.

## Methods
