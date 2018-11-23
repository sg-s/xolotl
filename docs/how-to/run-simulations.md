This document describes how to run simulations using xolotl. 


# Running simulations 
`xolotl` is designed to solve for state variables of [conductance-based](http://www.scholarpedia.org/article/Conductance-based_models) neuronal and network models.
The voltage across the membrane $V$ is given by the conservation of current equation.

$$C_m \frac{dV}{dt} = -\sum_i I_i$$

where $C_m$ is the specific membrane capacitance and $I_i$ is a specific transmembrane current. Each current takes the form

$$I_i = g_i(V) (V - E_i)$$

where $g_i(V)$ is the instantaneous conductance and $E_i$ the ionic reversal potential. In many models, the conductance $g_i(V)$ is given by

$$g_i(V) = \bar{g}_i m_i^{p_i} h_i^{q_i}$$

where $\bar{g}_i$ is the maximal conductance in Siemens per unit area and $m$ and $h$ are gating variables $\in[0, 1]$. The gating variables themselves are defined by differential equations which depend on the membrane potential. These equations are nonlinear and usually quite stiff. For these reasons, numerical simulations are required to adequately solve them.

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

## The outputs of `x.integrate`

### (1) Voltage or Injected current
When the `x.V_clamp` property is not set, the first output of `x.integrate` is the voltage trace in the form of a `nSteps x nComps` matrix where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. The number of time steps in the output is determined by the simulation time and the output time step `x.t_end` and `x.dt`.

### (2) Calcium
The calcium trace is in the form of a `nSteps x 2*nComps` matrix where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. The first `nComps` columns are the intracellular calcium concentration (in $\mu$M) for each compartment in the serialized `xolotl` object tree. The next set of `nComps` columns are the calcium reversal potential (in mV).

### (3) Currents

### (4) Mechanism variables

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

## Voltage clamping compartments

## Switching solvers

```matlab
x.solver_order = 4; % uses Runge Kutta 4
x.solver_order = 0; % default, uses exponential Euler
```

### The exponential Euler method
The exponential Euler method is a time-discrete solution to differential equations of the form:

$$\tau_V \frac{dV}{dt} = V_\infty - V$$

where $V = V(V, t)$ is the state variable and $\tau_V$ and $V_\infty$ are functions of $V$. This equation can be solved as follows:

Rearrange equation
$$\tau_V \frac{dV}{dt} = -(V + V_\infty)$$

Divide by $V - V_\infty$, multiply by $\tau_V$
$$ \frac{dV}{V-V_\infty} = -\frac{dt}{\tau_V}$$

Integrate, using the relation $\ln(x) = \int \frac{1}{x}dx$
$$\ln(V-V_\infty) = -\frac{t}{\tau_V} + \mathrm{const.}$$

Given a constant term $V(t_0)$, the equation can be written
$$V(t) = V_\infty + (V(t_0) - V_\infty) \exp \Big(-\frac{t-t_0}{\tau_V}\Big)$$

For a time step $\Delta t$, the voltage $V$ at time $t + \Delta t$ can be approximated from the voltage at time $t$, where $V_\infty = V_\infty \big(V(t)\big)$ and $\tau_V = \tau_V\big(V(t)\big)$
$$V(t + \Delta t) = V_\infty + (V(t) - V_\infty) \exp \Big(-\frac{\Delta t}{\tau_V}\Big)$$

This approximation is more accurate than a first order Euler method approximation, and is significantly faster than higher order (*viz.* Runge-Kutta) methods.
> See Ch. 5, Dayan, P. and Abbott, LF (2001) Theoretical Neuroscience, MIT Press for more information

### The Runge-Kutta fourth-order method

The [Runge-Kutta methods](http://lpsa.swarthmore.edu/NumInt/NumIntFourth.html) are extensions of forward Euler to higher derivative orders. Given a differential equation in the form

$$\frac{dV}{dt} = V'(t) = f(V, t)$$

with some initial condition $V(t_0) = V_0$. The first order (Euler) approximation for $V(t + \Delta t)$ given $V(t)$ is

$$V(t + \Delta t) = V(t) + f(V, t) \Delta t$$

Euler's method is accurate $\propto \Delta t$ per step. The Runge-Kutta fourth-order method uses four coefficients $k_1, k_2, ...$ to extend this method to accuracy of $\propto (\Delta t) ^4$ per step at the cost of speed. The coefficients are, for $V = V(t)$,

$$ k_1 = f(V, t)$$
$$k_2 = f\Big(V + k_1 \frac{\Delta t}{2}, ~t  + \frac{\Delta t}{2}\Big)$$
$$k_3 = f\Big(V + k_2 \frac{\Delta t}{2}, ~t  + \frac{\Delta t}{2}\Big)$$
$$k_4 = f\Big(V + k_3 \Delta t, ~t  + \Delta t \Big)$$

The time-evolution formula for the Runge-Kutta fourth order method is

$$V(t + \Delta t) = V(t) + \frac{k_1 + 2k_2 + 2k_3 + k_4}{6} \Delta t$$

The method is more accurate because slope approximations at fractions of $\Delta t$ are being taken and averaged. The method is slower because the four coefficients must be computed during each integration step.
