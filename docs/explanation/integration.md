This document describes how xolotl solves the [Ordinary Differential Equations](https://en.wikipedia.org/wiki/Ordinary_differential_equation) (ODEs) in its models.


`xolotl` is designed to solve for state variables of
[conductance-based](http://www.scholarpedia.org/article/Conductance-based_models) neuronal and network models.
The voltage across the membrane $V$ is given by

$$C_m \frac{dV}{dt} = -\sum_i I_i$$

where $C_m$ is the specific membrane capacitance and $I_i$ is a specific transmembrane current. Each current takes the form

$$I_i = g_i(V) (V - E_i)$$

where $g_i(V)$ is the instantaneous conductance and $E_i$ the ionic reversal potential. In many models, the conductance $g_i(V)$ is given by

$$g_i(V) = \bar{g}_i m_i^{p_i} h_i^{q_i}$$


where $\bar{g}_{i}$ is the maximal conductance in Siemens per unit area and $m$ and $h$ are gating variables $\in[0, 1]$. The gating variables themselves are defined by differential equations which depend on the membrane potential. These equations are nonlinear and usually quite stiff. For these reasons, bespoke integration schemes are typically used to solve them.

## The Exponential Euler method


The exponential Euler method is a time-discrete solution to differential equations of the form:

$$\tau_V \frac{dV}{dt} = V_\infty - V$$

where $V = V(V, t)$ is the state variable and $\tau_V$ and $V_\infty$ are functions of $V$. This equation can be solved as follows:

Rearrange equation
$$\tau_V \frac{dV}{dt} = -(V - V_\infty)$$

Divide by $V - V_\infty$, multiply by $\tau_V$
$$ \frac{dV}{V-V_\infty} = -\frac{dt}{\tau_V}$$

Integrate, using the relation $\ln(x) = \int \frac{1}{x}dx$
$$\ln(V-V_\infty) = -\frac{t}{\tau_V} + \mathrm{const.}$$

Given a constant term $V(t_0)$, the equation can be written
$$V(t) = V_\infty + (V(t_0) - V_\infty) \exp \Big(-\frac{t-t_0}{\tau_V}\Big)$$

For a time step $\Delta t$, the voltage $V$ at time $t + \Delta t$ can be approximated from the voltage at time $t$, where $V_\infty = V_\infty \big(V(t)\big)$ and $\tau_V = \tau_V\big(V(t)\big)$
$$V(t + \Delta t) = V_\infty + (V(t) - V_\infty) \exp \Big(-\frac{\Delta t}{\tau_V}\Big)$$

This approximation is more accurate than a first order Euler method approximation, and is significantly faster than higher order (*viz.* Runge-Kutta) methods.

!!! info "For more information"
    See Ch. 5, Dayan, P. and Abbott, LF (2001) Theoretical Neuroscience, MIT Press. You can read the full book [here](http://www.gatsby.ucl.ac.uk/~lmate/biblio/dayanabbott.pdf)

### Where this method is used

The exponential Euler method is used (when [`solver_order`](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#solver_order) = 0)

* to integrate the gating variables (`m` and `h` in every conductance). This method is defined in the conductance class.
* to integrate the voltage in compartments (for compartments that are not part of multi-compartment models)
* to integrate the Calcium levels in compartments (defined in some mechanisms)

## The Runge-Kutta fourth-order method

The [Runge-Kutta methods](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods)
are extensions of forward Euler to higher derivative orders.
Given a differential equation in the form

$$\frac{dV}{dt} = V'(t) = f(V, t)$$

with some initial condition $V(t_0) = V_0$. The first order (Euler) approximation for $V(t + \Delta t)$ given $V(t)$ is

$$V(t + \Delta t) = V(t) + f(V, t) \Delta t$$

Euler's method is accurate $\propto \Delta t$ per step.
The Runge-Kutta fourth-order method uses four coefficients
$k_1, k_2, ...$ to extend this method to accuracy of
$\propto (\Delta t) ^4$ per step at the cost of speed.
The coefficients are, for $V = V(t)$,

$$ k_1 = f(V, t)$$
$$k_2 = f\Big(V + k_1 \frac{\Delta t}{2}, ~t  + \frac{\Delta t}{2}\Big)$$
$$k_3 = f\Big(V + k_2 \frac{\Delta t}{2}, ~t  + \frac{\Delta t}{2}\Big)$$
$$k_4 = f\Big(V + k_3 \Delta t, ~t  + \Delta t \Big)$$


The time-evolution formula for the Runge-Kutta fourth order method is

$$V(t + \Delta t) = V(t) + \frac{k_1 + 2k_2 + 2k_3 + k_4}{6} \Delta t$$

The method is more accurate because slope approximations at fractions of $\Delta t$ are being taken and averaged. The method is slower because the four coefficients must be computed during each integration step.

### Where this method is used

The Runge-Kutta 4th order method is used when (when [`solver_order`](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#solver_order) = 4) for components that support this method. If any component does not support it, an error will be thrown.

## The Euler method

Euler's method is the most basic explicit method for
solving numerical integration problems of ordinary
differential equations, and is the simplest Runge-Kutta
method (i.e. it's 1st order). It is fast but inaccurate and unstable. Given a differential equation

$$\frac{dV}{dt} = f(V)$$

with a known initial condition $V(t)$, the next step is determined by the evolution equation

$$V(t + \Delta t) = V(t) + \Delta t f(V(t))$$

This process can be iterated to determine the trajectory of $V$ with accuracy on the order of $\mathcal{O}(\Delta t)$.

### Where this method is used

Some mechanisms may implement this method.

## The Crank-Nicolson method

The Crank-Nicolson method ([Crank & Nicolson 1947](https://link.springer.com/article/10.1007%2FBF02127704)) is based on the trapezoidal rule. It gives second-order convergence in time by using a combination of the forward Euler method at time $t$ and a backward Euler method at $t+\Delta t$.

For a compartment $n$ with upstream compartment $n-1$ and downstream compartment $n+1$ (it is part of a multi-compartment cable), the equation of state is

$$ \frac{d V_n}{dt} = B_n V_{n-1} + C_n V_n + D_n V_{n+1} + F_n $$

where

$$ B_n = C_m^{-1} g_{n-1, n} $$
$$ C_n = -C_m^{-1} \sum g_n^{(i)} + g_{n-1, n} + g_{n, n+1} $$
$$ D_n = C_m^{-1} g_{n, n+1} $$
$$ F_n = C_m^{-1} \sum g_n^{(i)} E^{(i)} + \frac{I_{ext}}{A_n} $$

Here, $C_m$ is the membrane capacitance of compartment $n$, $g_{n, m}$ is the axial conductance from compartment $n$ to compartment $m$,
$g_n^{(i)}$ is the conductance of conductance $i$ in compartment $n$ and $E^{(i)}$ is the reversal potential of conductance $i$. Additionally,
$I_{ext}$ is the injected current, which here is scaled by the surface area $A_n$ of compartment $n$.

By defining,

$$ V_n (t + \Delta t) = V_n + \Delta t $$

the equation of state above can be rewritten in the implicit form

$$ \Delta V_n = \left( B_n V_{n-1}(t + z \Delta t) + C_n V_{n}(t + z \Delta t) + D_n V_{n+1}(t + z \Delta t) \Delta t \right) $$

where $z = 0.5$ for the Crank-Nicolson method. This equation must be solved to determine $\Delta V_n$. For multi-compartment models, this involves writing an algebraic expression that sequentially solves starting at one end of the cable and proceeding to the other end(s). Then, a second sweep from the tips back to the soma allows for explicit solving at the next time point.

For branching morphologies that do not branch back into itself (that is, there is only one path with backtracking from soma to tip for each tip), tridiagonal matrix solvers (viz. Thomas algorithm, Hines matrix solvers), can be used to dramatically speed up the calculations. Currently, xolotl does not support solving branching morphologies.

### Where this method is used

The Crank-Nicolson method is the default for multi-compartment models. The Runge-Kutta method, while sufficient to integrate multi-compartment models accurately is orders of magnitude slower than the Crank-Nicolson method.

## Euler-Maruyama method

The [Euler-Maruyama method](https://en.wikipedia.org/wiki/Euler%E2%80%93Maruyama_method) approximates the numerical solution of a stochastic differential equation.
It is a generalization of the first-order Euler method for ordinary differential equations.

The equation of state for conductance-based models is

$$ C_m \frac{dV}{dt} = \sum_i I_i = \sum_i g_i (V) (V - E_i) $$

This equation is deterministic, and is an approximation of the more realistic case, which is a stochastic system with $N$ independent ion channels.
When $N$ is very large, this is a good approximation, since the law of large numbers implies that the standard error in the proportion of channels open is very small.
When $N$ is small, stochasticity arising from finite numbers of channels can be modeled by an approximate Langevin formulation proposed by [Fox & Lu 1994](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.49.3421).

$$ C_m \frac{dV}{dt} = \sum_i g_i (V, \xi_i) (V - E_i) $$

The fluctuation term $\xi_i$ is understood to be an uncorrelated Gaussian
random variable with zero mean and unit variance.
Since $g_i(V, \xi_i)$ is understood to be some product of gating variables,
the noise is included in the gating variable equation of state (subunit noise).

For a generic gating variable $x = x(t)$ for some (temporarily) fixed $V = V(t)$,
the equation of state without noise is an ordinary differential equation (ODE)

$$ f(x) = \frac{dx}{dt} = \frac{x_{\infty}(V) - x}{\tau_x(V)} $$

and with noise, it is a stochastic differential equation (SDE)

$$ f(x | \xi_x) = \frac{dx}{dt} = \frac{x_{\infty}(V) - x}{\tau_x(V)} + \xi_i(t) = f(x) + \xi_x(t) $$

The solution to the SDE can be approximated using the Euler-Maruyama method, which is a modified version of Euler's method for ODEs.

For a time step $\Delta t$ and V = V(t), the Euler approximation to the solution of the ODE is

$$ x(t + \Delta t) = x(t) + \Delta t \cdot f(x(t)) $$

The Euler-Maruyama approximation to the solution of the SDE is

$$ x(t + \Delta t) = x(t) + \Delta t \cdot f(x(t)) + \Delta \xi_x(t) $$

Since we are solving these equations numerically with a fixed time step,
we fetch a new independent and identically distributed Gaussian random number at each time step,
such that $\Delta \xi_x(t)$ is some function of $\xi_x(t)$.
Intuitively, $\Delta \xi_i(t)$ will incorporate the time step, the number of channels, and the dynamics of $x$.

Since the noise is per-channel, we calculate the number of channels using an approximation.

$$ N = \mathrm{round} \left( \frac{\bar{g}_i A}{g_0} \right) $$

Here, $N$ is the estimated number of channels (rounded to a natural number),
$\bar{g}_i$ is the maximal conductance of the $i$th channel,
$A$ is the surface area of the compartment,
and $g_0 = 20 \times 10^{-6}$ microsiemens, the conductance of a single channel.

Then, the Euler-Maruyama approximation for the stochastic gating variable as a function of time is

$$ x(t + \Delta t) = x(t) + \Delta t \frac{x_{\infty}(V) - x(t)}{\tau_x(V)} + \sqrt{ \frac{\Delta t}{\tau_x(V)} \cdot \left( \frac{x + x_{\infty}(V)- 2 x \cdot x_{\infty}(V)}{N} \right) \xi_x(t) } $$

### Where this method is used

This method is automatically used when a model includes "conductance" noise,
that is, noise caused by fluctuations in the opening and closing of ion channels.
For "current" noise, use current clamp with a pseudorandom injected current constructed prior to simulation.

This method is consistent with [Goldwyn & Shea-Brown 2011](https://journals.plos.org/ploscompbiol/article/file?id=10.1371/journal.pcbi.1002247&type=printable)
and [Sengupta, Laughlin, and Niven 2010](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.81.011918).

## Bibliography

* Theoretical Neuroscience. Dayan and Abbott. You can read the full book [here](http://www.gatsby.ucl.ac.uk/~lmate/biblio/dayanabbott.pdf)
* Crank-Nicolson Method. [Crank & Nicolson 1947](https://link.springer.com/article/10.1007%2FBF02127704).
* Langevin formulation of subunit noise. [Fox & Lu 1994](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.49.3421).
* Discussion of noise in Hodgkin-Huxley models. [Goldwyn & Shea-Brown 2011](https://journals.plos.org/ploscompbiol/article/file?id=10.1371/journal.pcbi.1002247&type=printable) & [Sengupta, Laughlin, and Niven 2010](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.81.011918).
