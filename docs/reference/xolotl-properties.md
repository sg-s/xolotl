Every xolotl object has the following properties listen in this document. To access a property, use dot notation, i.e.:

```
x.verbosity
```

You can view all the properties of a xolotl object using the built-in `properties` command:

```
properties(xolotl)
% will display a list of properties
```

# Controlling simulation 

## `V_clamp`

## `approx_channels`

## `I_ext`

## `closed_loop`

## `dt` and `sim_dt`

## `t_end`

## `output_type`

# Controlling verbosity

## `verbosity`

|  |  |
| ------- | ----- |
| Default | 0 |
| Allowed values | +ve numbers |
| Type | double | 

`verbosity` is a positive integer that controls how verbose xolotl is when running simulations. Set to a large positive number to get more verbose output, useful for debugging. 

# Customization 

## `pref`

# Exploring the model

## `Children`





* The `closed_loop` flag (false or true) determines whether initial conditions should be reset before a new simulation. If `closed_loop` is true, successive simulations will use the current state of the `xolotl` object (e.g. the end state of the previous simulation if you run `integrate` twice in a row).
* The `approx_channels` flag (0 or 1) determines whether approximations to computing gating functions should be used. Look-up tables and approximations to the exponential function significantly increase computational speed, but decrease accuracy, especially at high temporal resolution in the data.
* The `solver_order` flag takes the values 0 or 4. In the 0 case, standard solvers are used (exponential Euler). In the 4 case, a Runge-Kutta 4th order method is used instead. This method is slower but more accurate.
* The `output_structure` flag (0, 1, or 2) determines if outputs from the `integrate` function should be separate matrices (0) or organized in a structure (1), or organized in a structure and enable spike-detection in C++ code (2). The 0 option is useful when you only want a few outputs or don't care about lots of variable names. The latter options are useful when it's important to keep all the output data organized. In addition, the 2 option saves memory at the expense of detail.

### Numerical Properties

* The `dt` value stores the fixed time step (default $50 \times 10^{-3}~\mathrm{ms}$) for outputs from simulation. Note that this is not the same as `sim_dt`. This value determines the number of time steps in the output vectors. If `dt` and `sim_dt` differ, the simulation vector is interpolated before being output -- useful for running ultra-high definition simulations but not saving all that data.
* In contrast, `sim_dt` is the actual fixed time step for the simulation (default $50 \times 10^{-3}~\mathrm{ms}$).
* `t_end` is the simulation time (default $5 \times 10^{3}~\mathrm{ms}$).

* The `temperature` property holds the *in-silico* preparation temperature (default 11 deg. C). This property only matters when using temperature-sensitive conductances or mechanisms.
* `temperature_ref` holds the "default" temperature so that $Q_{10}$ values can be used.
* The `I_ext` property stores the current to be injected as a scalar, vector, or matrix.
* The `V_clamp` property stores the voltage of clamped compartments as a matrix of `nSteps x nComps` where `nSteps` is the number of time-steps (`x.t_end * x.dt`) and `nComps` is the number of compartments in the `xolotl` object tree.

### Non-Numerical Properties
* The `manipulate_plot_func` property contains a cell of function handles which correspond to all plotting functions that are called when a property is changed while using the `manipulate` functionality. It defaults to the built-in `x.plot` function.
* The `pref.m` file contains editable preferences for your `xolotl` installation.
