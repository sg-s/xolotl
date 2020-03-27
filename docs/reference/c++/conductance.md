
This document describes the "conductance" C++ class.
This class describes objects that are conductances, or
populations of ion channels.

This is an abstract class, and concrete implementations
of ion channel types need to inherit from this class and
define certain attributes like their activation functions.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |

## Properties

### `container`

| type | default | user-accessible |
| --------  | ------ | -------  |
| compartment* |  NULL | no |

### `gbar`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | yes |

The maximal conductance of this channel type (in $uS/mm^2$). This
is typically exposed to the user as a parameter to set and modify.


### `gbar_next`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |

### `g`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |

The instantaneous conductance of this channel type.
This is a product of `gbar` and the activation and
inactivation variables.

$$g = \bar{g} m^p h^q$$

### `E`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | yes |

The reversal potential of this channel type.

### `m`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |

The activation variable of this channel type.

### `h`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  1 | no |


The inactivation variable of this channel type.

### `verbosity`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |


A flag that tells this channel how verbose it should be.
This should not be exposed to the user, since it it
broadcast to all components from `xolotl.verbosity`.


## Methods



### buildLUT

**Function Signature**

```C++
void buildLUT(double approx_channels) 
```

**Description**


This method constructs a look up table (LUT)
that is used to estimate $m_{inf}$ and other
functions of the voltage. Since these functions 
are repeatedly evaluated, it is often faster to compute
them for some values of the voltage once, store these
values in a table, and use this table subsequently. 
This is an approximation since the voltage is rounded
off to the nearest value in the look-up table, uses
a little more memory, but can be much faster. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L200)

-------



### fast_pow

**Function Signature**

```C++
inline double fast_pow(double x, int a) 
```

**Description**



This method is a dirty hack to speed up computing
exponents in C++. This requires that the power that a 
number is raised to be an integer (0-8)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L247)

-------



### fast_exp

**Function Signature**

```C++
inline double fast_exp(double x) 
```

**Description**


This method constitutes a dirty hack which 
is a faster way to compute exp(x)
but is less precise



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L285)

-------



### getCurrent

**Function Signature**

```C++
double getCurrent(double V) 
```

**Description**



The method returns the current that flows through
this channel at this moment. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L300)

-------



### checkSolvers

**Function Signature**

```C++
void checkSolvers(int solver_order) 
```

**Description**





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L306)

-------



### mdot

**Function Signature**

```C++
double mdot(double V, double Ca, double m_) 
```

**Description**


This method defines the rate of change of the `m` variable
of this conductance. This definition is used when `integrateMS` is used. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L321)

-------



### hdot

**Function Signature**

```C++
double hdot(double V, double Ca, double h_) 
```

**Description**


This method defines the rate of change of the `h` variable
of this conductance. This definition is used when `integrateMS` is used. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L329)

-------



### m_inf

**Function Signature**

```C++
double m_inf(double V, double Ca)
```

**Description**


This method defines the activation curve of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L338)

-------



### h_inf

**Function Signature**

```C++
double h_inf(double V, double Ca)
```

**Description**


This method defines the inactivation curve of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L345)

-------



### tau_m

**Function Signature**

```C++
double tau_m(double V, double Ca)
```

**Description**


This method defines the dependence of the timescale 
of the activation variable on the voltage of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L353)

-------



### tau_h

**Function Signature**

```C++
double tau_h(double V, double Ca)
```

**Description**


This method defines the dependence of the timescale 
of the inactivation variable on the voltage of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L361)

-------



### gaussrand

**Function Signature**

```C++
double gaussrand() 
```

**Description**


This method implements a very fast Gaussian random
number generator. This is much faster than the 
built-in generators in the C++ `<random>` header, and
is copied from Knuth and Marsaglia.

For the original source, see
"A Convenient Method for Generating Normal Variables"
SIAM Rev., 6(3), 260–264.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L374)

-------



### connect

**Function Signature**

```C++
void connect(compartment *pcomp_) 
```

**Description**


This method "connects" a conductance object to a compartment
object. This sets the `container` property of the conductance,
so the channel knows which compartment contains it. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L407)

-------



### integrate

**Function Signature**

```C++
void integrate(double V, double Ca) 
```

**Description**


This method integrates the conductance object using
the exponential Euler method. This is the default
integration method used by xolotl. If an exact solution
is to be calculated (i.e.,`approx_m = 0` and `approx_h=0`)
then `m` and `h` are updated using the exponential Euler
equation using function evaluations of the activation 
functions at this voltage and Calcium.

Otherwise, the lookup table is used to update `m` and `h`
in this channel. 

Note that this method is defined as virtual, so it can be
overridden by integration methods specified in a specific
conductance. 

**See Also** 

* [virtual methods in C++](http://www.cplusplus.com/doc/tutorial/polymorphism/)



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L453)

-------



### integrateLangevin

**Function Signature**

```C++
void integrateLangevin(double V, double Ca) 
```

**Description**


This method integrates the conductance object using
the Euler-Maruyama method. The integration method
used here is consistent with the methods used in
[Goldwyn and Shea-Brown 2011](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247)
and with [Sengupta, Laughlin and Niven](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.81.011918)

Briefly, this method follows the approximate Langevin
formulation of the underlying stochastic system
formed by N independent channels that have 
independent gating kinetics. It can be thought of
as the deterministic ODE, with an additive noise term
whose variance scales with the inverse square root of
the number of channels. The number of channels is 
computed automatically from the channel density
and the area of the compartment. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L555)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(int k, double V, double Ca) 
```

**Description**



This method integrates a channel object using a multi-step
solver (MS = "multi-step"). The "sub-step" is indicated in 
the integer k, which is the first input to this method. 

The multi-step solver that is used here is a Runge-Kutta 4th
order solver. Thus, k can have values up to 4. 

Based on `k`, different elements of the arrays `k_m` and `k_h`
are calculated and stored. At each step, the derivative functions
`mdot` and `hdot` are computed. 

**See Also**

* [The Runge Kutta Method](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods)




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L656)

-------

