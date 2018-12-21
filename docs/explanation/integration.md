This document describes how xolotl solves the [Ordinary Differential Equations](https://en.wikipedia.org/wiki/Ordinary_differential_equation) (ODEs) in its models. 


`xolotl` is designed to solve for state variables of 
[conductance-based](http://www.scholarpedia.org/article/Conductance-based_models) neuronal and network models.
The voltage across the membrane $V$ is given by

$$C_m \frac{dV}{dt} = -\sum_i I_i$$

where $C_m$ is the specific membrane capacitance and $I_i$ is a specific transmembrane current. Each current takes the form

$$I_i = g_i(V) (V - E_i)$$

where $g_i(V)$ is the instantaneous conductance and $E_i$ the ionic reversal potential. In many models, the conductance $g_i(V)$ is given by

$$g_i(V) = \bar{g}_i m_i^{p_i} h_i^{q_i}$$


where $\bar{g} i$ is the maximal conductance in Siemens per unit area and $m$ and $h$ are gating variables $\in[0, 1]$. The gating variables themselves are defined by differential equations which depend on the membrane potential. These equations are nonlinear and usually quite stiff. For these reasons, bespoke integration schemes are typically used to solve them. 

## The Exponential Euler method


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

### Where this method is used


## The Runge-Kutta fourth-order method

The [Runge-Kutta methods](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods) are extensions of forward Euler to higher derivative orders. Given a differential equation in the form

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

## The Euler method 

### Where this method is used

## The Crank-Nicholson Method 

### Where this method is used 


## Bibliography 

* Theoretical Neuroscience. Dayan and Abbott. You can read the full book [here](http://www.gatsby.ucl.ac.uk/~lmate/biblio/dayanabbott.pdf)