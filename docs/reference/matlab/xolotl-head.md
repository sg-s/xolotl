
# The xolotl class

This document describes the "xolotl" class. This is a MATLAB
class and the primary way you will interact with your
simulations. 

## Properties

Every xolotl object has the following properties listen in this document. To access a property, use dot notation, i.e.:

```matlab
x.verbosity
```

You can view all the properties of a xolotl object using the built-in [properties](https://www.mathworks.com/help/matlab/ref/properties.html) command:

```matlab
properties(xolotl)
% will display a list of properties
```

### `approx_channels`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 0  |     0, 1 | double | 

`approx_channels`  determines whether approximations
to computing gating functions should be used. 
Look-up tables and approximations to the exponential 
function significantly increase computational speed, 
but decrease accuracy, especially at high temporal
resolution in the data.

### `closed_loop`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| true  |     true, false | logical | 

`closed_loop`  determines whether initial conditions 
should be reset before a new simulation. If `closed_loop`
is true, successive simulations will use the current 
state of the `xolotl` object (e.g. the end state of the
previous simulation if you run `integrate` twice in a row).

### `dt` and `sim_dt`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| .05  |     +ve numbers | double | 

`dt` value stores the fixed time step for outputs from 
simulation. Note that this is not the same as `sim_dt`. 
This value determines the number of time steps in the 
output vectors. If `dt` and `sim_dt` differ, the simulation
vector is interpolated before being output -- useful for 
running ultra-high definition simulations but not saving
all that data.

`dt` must be a integer multiple of `sim_dt`. If it is 
not, an error will be thrown. 

### `V_clamp`


| Default | Allowed values | Type |
| ------- | ----- | ----- |
| NaN  |     matrix, vector, scalar | double | 

When `V_clamp` is not a `NaN`, xolotl will assume that you are
running the simulation in voltage clamp mode. `V_clamp` must be either:

* a vector as long as `x.Children` (the # of compartments)
* a matrix of size (n_steps,n_compartments) where n_steps is the number
 of steps in the integration (which is x.t_end/x.sim_dt)

Incorrectly sized `V_clamp` will throw an error. 

You cannot simultaneously inject current and voltage clamp 
any compartment. 

### `I_ext`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 0  |     matrix, vector, scalar | double | 

`I_ext` must be either:

* a vector as long as `x.Children` (the # of compartments)
* a matrix of size (n_steps,n_compartments) where n_steps is the number
 of steps in the integration (which is x.t_end/x.sim_dt)

Incorrectly sized `I_ext` will throw an error. 

You cannot simultaneously inject current and voltage clamp 
any compartment. 

### `solver_order`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 0  |   0, 4 | double | 


When `solver_order` is 0, standard solvers are used
(exponential Euler). When it is 4, a Runge-Kutta 4th
order method is used instead. This method is slower
but more accurate.


### `t_end`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 5e3  |    +ve integers | double | 

Specify the time, in ms, for which to simulate. Make sure
that `t_end` is an integer multiple of `sim_dt`


### `output_type`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 0  |    0,1,2 | double | 

`output_type` determines if outputs from the `integrate` 
function should be separate matrices (0) or organized 
in a structure (1), or organized in a structure and 
enable spike-detection in C++ code (2). The 0 option is 
useful when you only want a few outputs or don't care 
about lots of variable names. The latter options are 
useful when it's important to keep all the output data 
organized. In addition, the 2 option saves memory at the 
expense of detail.

### `temperature` and `temperature_ref`

`temperature` specifies the temperature at which you want
to perform simulations. `temperature_ref` holds the "default" 
temperature so that $Q_{10}$ values can be used. These
values only affect components that are temperature sensitive. 


### `verbosity`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 0  |     +ve numbers | double | 

`verbosity` is a positive integer that controls how verbose
xolotl is when running simulations. Set to a large positive 
number to get more verbose output, useful for debugging. 


### `pref`

`x.pref` contains a structure that contains settings that 
determine the behaviour of some methods. You can change these 
settings temporarily by modifying this structure. To make these
changes persist across sessions, edit the `pref.m` file.

You can also add your own data to `x.pref`, which your 
functions can use. This is a way to pass metadata and other 
data along with this model to other functions. 


### `Children`

`Children` contains a list of all compartments in the 
current xolotl model. xolotl only allows you to add 
objects of type "compartment" to it. Therefore, 
`x.Children` will give you a list of compartments in 
the model.

## Methods