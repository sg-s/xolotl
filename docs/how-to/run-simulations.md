This document describes how to run simulations using xolotl.




## Changing the time step and duration

In the case where the simulation time step (`sim_dt`) and the output time step (`dt`) are identical, the `dt` property only needs to be set.

```matlab
% set the simulation time to 10 seconds
x.t_end 	= 10e3; % ms
% set the time-step to 1 microsecond
x.dt 		= 1e-3; % ms
```

The `sim_dt` property determines the number of time-steps actually computed. The `dt` property determines the number of time-steps output. In the following example, the computer would perform 1 million (`x.t_end/x.sim_dt`) iterations but the output vector (or matrix) `V` is only `1000 x nComps` where `nComps` is the number of compartments in the `xolotl` object tree.

```matlab
% perform 1e6 iterations, interpolate at a ratio of 1/1000
x.t_end 	= 1000; % 1000 milliseconds
x.sim_dt	= 1e-3; % 0.001 milliseconds
x.dt 		= 1;	% 1 millisecond
V 			= x.integrate;
```

## Closed loop vs. open loop
The `closed_loop` flag (false or true) determines whether initial conditions should be reset before a new simulation. If `closed_loop` is true, successive simulations will use the current state of the `xolotl` object (e.g. the end state of the previous simulation if you run `integrate` twice in a row).

```matlab
% use current state of model as initial conditions
x.closed_loop = true
V = x.integrate;
```

You can set the initial conditions by setting the desired properties.

```matlab
% set the voltage to start at -50 mV in compartment 'comp'
x.comp.V = -50;
```

To quickly save the state of a network and return back to it later (such as for running multiple simulations)
you can use the [snapshot](snapshots.md) functionality.

## The outputs of `x.integrate`

### (1) Voltage or Injected current
When the `x.V_clamp` property is not set, the first output of `x.integrate` is the voltage trace in the form of a `nSteps x nComps` matrix where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. The number of time steps in the output is determined by the simulation time and the output time step `x.t_end` and `x.dt`.

### (2) Calcium
The calcium trace is in the form of a `nSteps x 2*nComps` matrix where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. The first `nComps` columns are the intracellular calcium concentration (in $\mu$M) for each compartment in the serialized `xolotl` object tree. The next set of `nComps` columns are the calcium reversal potential (in mV).

### (3) Mechanism variables

Mechanisms such as integral controllers produce output traces. All mechanism
traces are stored in an $n \times m$ matrix where $n$ is the number of time steps
(`x.t_end / x.dt`) and $m$ is the number of mechanisms producing traces. Mechanism
traces are ordered exactly the same as in the `xolotl` object, meaning by compartment
and then by conductance.

### (4) Currents

Currents are stored in an $n \times c$ matrix where $n$ is the number of time steps
(`x.t_end / x.dt`) and $c$ is the number of conductances in the network. Currents
are ordered exactly the same as in the `xolotl` object, meaning by compartment and
then alphabetically.

!!! Note "Plotting mechanisms and currents"
    The example script `demo_stg` contains code that plots currents vs. time and `demo_integral_control` contains code that plots mechanisms vs. time.

## Inject current into compartments
Injected current is mediated by the `I_ext` property of the `xolotl` object.

If `I_ext` is a scalar, that amount of current in $\mathrm{nA}$ is injected into every compartment at all time.

```matlab
% inject 0.2 nA into each compartment
x.I_ext = 0.2;
```

If `I_ext` is a vector the length of the number of compartments, constant current will be added to each compartment in order (based on the serialized `xolotl` object).

```matlab
% add current to only the first compartment
x.I_ext = [0.2, 0];
```

If `I_ext` is a matrix, it should be of size `nSteps x nComps` where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. Current is added at each time step to each compartment in order (based on the serialized `xolotl` object).

```matlab
% add a variable current into one of two compartments
nSteps 		= x.t_end / x.dt;
I_ext		= zeros(nSteps, 2);
I_ext(:, 1) = 0.2 * rand(nSteps, 1);
x.I_ext 	= I_ext;
```


## Switching solvers

```matlab
x.solver_order = 4; % uses Runge Kutta 4
x.solver_order = 0; % default, uses exponential Euler
```




## See Also

* [xolotl.integrate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#integrate)
* [xolotl.approx_channels](https://xolotl.readthedocs.io/en/master/reference/xolotl-properties/#approx_channels)
* [xolotl.closed_loop](https://xolotl.readthedocs.io/en/master/reference/xolotl-properties/#closed_loop)
* [xolotl.output_type](https://xolotl.readthedocs.io/en/master/reference/xolotl-properties/#output_type)