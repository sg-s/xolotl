
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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L194)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L241)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L280)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L295)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L302)

-------



### checkSolvers

**Function Signature**

```C++
void checkSolvers(int solver_order) 
```

**Description**





 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L309)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L324)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L332)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L341)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L348)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L356)

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

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L364)

-------



### gaussrand

**Function Signature**

```C++
double gaussrand() 
```

**Description**


This method defines the dependence of the timescale 
of the inactivation variable on the voltage of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 

double conductance::tau_h(double V, double Ca){return 1;}



// originally from Knuth and Marsaglia
// see "A Convenient Method for Generating Normal Variables"
// SIAM Rev., 6(3), 260–264.


 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp#L371)

-------

