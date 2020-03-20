
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

### `stochastic_channels`

| Default | Allowed values | Type |
| ------- | ----- | ----- |
| 0  |   0, 1 | double | 

When `stochastic_channels` is 0, it is assumed that
there are sufficiently many channels of every conductance
type, and that we can use a deterministic ODE to integrate
the gating kinetics of every channel. 

When `stochastic_channels` is 1, the deterministic ODE
governing channel gating is replaced by a stochastic
equation that is integrated using the [Euler-Maruyama](https://en.wikipedia.org/wiki/Euler%E2%80%93Maruyama_method)
following the approximate Langevin formulation. With 
this flag, the size of the neuron determines the number
of channels in every population, and thus determines the
noise added onto the deterministic dynamics. 


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

-------

### add


**Syntax**

```matlab
x.add('compartment','comp_name')
x.add(compartment,'comp_name')
x.add('compartment','comp_name',...)
x.add(Compartment,'comp_name')
```

**Description**

Adds a `cpplab` object to a `xolotl` object. The `add` method is the most important way you construct models.

- **`x.add('compartment','comp_name')`** adds a compartment to the xolotl object and names it `comp_name`.

- **`x.add(compartment,'comp_name')`** adds a compartment object (a cpplab object) to the xolotl object `x` and names it `comp_name`. Note that compartment is a cpplab object sourced from the `compartment.hpp` C++ file, and can contain children and be extensively modified.

- **`x.add('compartment','comp_name',...)`** adds a compartment to the xolotl object and names it `comp_name`. The compartment is then additionally configured using the parameters specified using Name Value syntax.

- **`x.add(Compartment, 'comp_name')`** adds a pre-defined cpplab object of class "compartment" to the xolotl object and names it `comp_name`. You cannot pass additional name-value arguments using this syntax.

**Technical Details**

`xolotl.add` checks that the compartment being added has a legal name
using `checkCompartmentName`. If so, it calls the `add` method 
in the `cpplab` superclass.




!!! info "See Also"
    * [cpplab.add](../cpplab/#add)
    * [xolotl.checkCompartmentName](../xolotl/#checkcompartmentname)





-------

###  benchmark

**Syntax**

```matlab
x.benchmark;
```

**Description**

performs a quick benchmarking of a given `xolotl` model.
`benchmark` first varies the simulation time step, and
measures how quickly the model integrates. It then
varies `t_end`, and measures how fast it integrates
at a fixed `sim_dt`.

It should produce a figure that looks something like this
(the exact figure will depend on the model and your hardware):

![](https://user-images.githubusercontent.com/6005346/50046554-1714f800-0073-11e9-9b1f-f136baff7976.png)






-------

### checkCompartmentName

**Syntax**

```matlab
TF = checkCompartmentName(self,comp_name)
```

**Description**

`checkCompartmentName` is used internally by `xolotl`
to verify that the compartment name you are using is valid and legal.  This method is called every time you add a compartment
to a `xolotl` object.

!!! warning
    Do not use `checkCompartmentName`, as it may be removed in a future release.



!!! info "See Also"
    * [xolotl.add](../xolotl/#add)





-------

### checkTree

**Syntax**

```matlab
x.checkTree
```

**Description**

This method checks that objects in the xolotl tree make sense
and are contained by objects that are allowed to contain them

The following rules are enforced:

| Object |   Legal container |
| ------- | --------------- |
| compartment | xolotl object |
| mechanism | any |
| conductance | compartment |
| synapse | compartment |

This method is called in xolotl.transpile() before
transpiling takes place




!!! info "See Also"
    * [xolotl.transpile](../xolotl/#transpile)
    * [xolotl.compile](../xolotl/#compile)





-------

### cleanup

**Syntax**

```matlab
xolotl.cleanup
x.cleanup
```

**Description**

A static method that cleans up all transpiled ``C++`` and compiled binary files.

!!! warning
    Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries.




!!! info "See Also"
    * [xolotl.compile](../xolotl/#compile)
    * [xolotl.transpile](../xolotl/#transpile)




-------

### compile

**Syntax**

```matlab
x.compile
```

**Description**

compiles a executable binary form a transpiled ``C++`` file.
These are stored in your ``xolotl`` directory. ``xolotl``
automatically compiles when t needs to. You can turn this


-------

### connect


**Syntax**

```matlab
x.connect('Comp1', 'Comp2')
x.connect('Comp1', 'Comp2',resistivity)
x.connect('Comp1', 'Comp2', SynapseObj)
x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp')
x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp','Parameter',Value...)
```

**Description**

Connects two compartments with a synapse.


- **`x.connect('Comp1', 'Comp2')`** connects two compartments, `Comp1` and `Comp2`, using reciprocal `Axial` synapses.


- **`x.connect('Comp1', 'Comp2',resistivity)`** connects two compartments, `Comp1` and `Comp2`, using reciprocal `Axial` synapses and specifies the axial resistivity.

- **`x.connect('Comp1', 'Comp2', SynapseObj)`** makes a synapse with presynaptic compartment `Comp1` and post-synaptic compartment `Comp2` using the synapse object `SynapseObj`. SynapseObj is a cpplab object that corresponds to a synapse.

- **`x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp')`** makes a synapse with presynaptic compartment `Comp1` and post-synaptic compartment `Comp2` using a synapse object that is generated on the fly using the C++ header file specified by `'path/to/synapse.hpp'`


- **`x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp','Parameter',Value...)`** makes a synapse with presynaptic compartment `Comp1` and post-synaptic compartment `Comp2` using a synapse object that is generated on the fly using the C++ header file specified by `'path/to/synapse.hpp'`, additionally configuring that object with parameters and values using name-value notation.


The following properties can be specified for most synapses:


| Name                 |   PropertyName |
|----------------------| -----------|
| Maximal conductance  |  `gmax`|
| Reversal potential   |  `E`|
| Activation variable  |  `s`|

This method supports tab completion. You should be able to press
tab to get a list of compartments to connect.



!!! info "See Also"
    * [xolotl.add](../xolotl/#add)





-------

### contributingCurrents

**Syntax**

```matlab
curr_index = xolotl.contributingCurrents(V, I)
```

**Description**

This static method calculates the contributions of each
current at every point in a voltage race. This is used
internally in `xolotl.plot` to color voltage traces.

where V is a vector of voltages, I is the corresponding matrix of currents



!!! info "See Also"
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.manipulate](../xolotl/#manipulate)





-------

### copy

Syntax:

```matlab
x2 = copy(x);
```

copies a xolotl object. ``copy`` creates an identical
copy of a xolotl object that can be manipulated separately.
Both copies will use the same binary to integrate,
unless you add a new component to one of them.

!!! warning
    * Some read-only properties in a xolotl object may not be copied over.
    * Do not make vectors of ``xolotl`` objects, as it may lead to undefined behavior.




!!! info "See Also"
    * [cpplab.copy](../cpplab/#copy)
    * [How to copy models](https://xolotl.readthedocs.io/en/master/how-to/copy-models/)






-------

### currentscape

**Syntax**

```matlab
x.currentscape()
norm_currents = x.currentscape()
```

**Description**

This method creates a "currentscape", or a visualization of the fractional
contribution of each current at each point in time 

When called with one output, no plot is created, but the raw normalized
inward and outward currents are returned. 

This method only works with single-compartment models. 

You can use manipulate to vary parameeters and see this update in real time

```matlab
x.manipulate_plot_func = {@x.currentscape};
x.manipulate('*gbar');
```




!!! info "See Also"
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.manipulate](../xolotl/#manipulate)
    * [[Visualization of currents in neural models with similar behavior and different conductance densities](https://elifesciences.org/articles/42722)](../[Visualization of currents in neural models with similar behavior and different conductance densities](https://elifesciences/#org/articles/42722))



-------

### fI

**Syntax**

```matlab
data = x.fI()
data = x.fI('Name',value...)
```

**Description**

This method computes the f-I (firing-rate vs current)
curve of a single compartment model. `data` is a structure containing the following fields:

* `I` vector of injected currents
* `f_up` firing rates when going up the curve
* `f_down` firing rates when going down the curve
* `CV_ISI_up` coefficient of variation of inter-spike intervals when going up the curve
* `CV_ISI_down` coefficient of variation of inter-spike intervals when going down the curve

The following optional parameters may be specified in name-value syntax:

| Name | Allowed Values | Default |
| ----- | ----------- | ---------- |
| I_min | any scalar | - .1 |
| I_max | any scalar | 1 |
| n_steps | +ve integer | 10 |
| t_end | +ve integers | 1e4 |



!!! info "See Also"
    * [xolotl.integrate](../xolotl/#integrate)





-------

### getGatingFunctions


**Syntax**

```matlab
[m_inf, h_inf, tau_m, tau_h] =  xolotl.getGatingFunctions(conductance)
```

**Description**

static method of `xolotl` that returns function handles
that represent the gating and activation functions of a
particular conductance.


`conductance` is a string that specifies a
conductance C++ header file. The outputs are function
handles that can be evaluated independently. This method
is used internally in `xolotl.show()`


This method supports tab-completion. You should be able to press
tab to get a list of conductances you can get the
gating function of.



!!! info "See Also"
    * [xolotl.show](../xolotl/#show)






-------

### go_to_examples


**Syntax**

```matlab
xolotl.go_to_examples
```

**Description**

A static method that goes to the folder that contains xolotl examples.



!!! info "See Also"
    * [xolotl.run_all_tests](../xolotl/#run_all_tests)






-------

### integrate

integrates a `xolotl` model.

**Syntax**

```matlab
x.output_type = 0;
V = x.integrate;
I_clamp = x.integrate;
[V, Ca] = x.integrate;
[V, Ca, mech_state] = x.integrate;
[V, Ca, mech_state, I] = x.integrate;
[V, Ca, mech_state, I, syn_state] = x.integrate;

x.output_type = 1;
results = x.integrate;

x.output_type = 2;
results_and_spiketimes = x.integrate;
```

**Description**

The outputs of integrate depend on the `output_type` property of `xolotl`.

| `output_type` value | outputs of `x.integrate` |
| ------------------- | ------------------------ |
| 0 (default) | up to 5 matrices of type double |
| 1 | only one output, a structure |
| 2 | only one output, a structure |


**Explanation of outputs**

When `output_type` is 0,

- `V` Voltage trace of every compartment. A matrix of size (nsteps, n_comps)
- `I_clamp` also returned in the first argument, this is the clamping current when a compartment is being voltage clamped. This can be inter-leaved with the voltage of other, non-clamped compartments.
- `Ca` Calcium concentration in every cell and the corresponding `E_Ca` (reversal potential of Calcium). A matrix of size (nsteps, n_comps)
- `mech_state` a matrix representing every dimension of every mechanism in the tree. This matrix has size (nsteps, NC), where NC depends on the precise controllers used, and is automatically determined.
- `I` the currents of every ion channel type in the model. This is a matrix of size (nsteps, n_cond)

When `output_type` is 1 or 2, the integration is performed requesting all outputs, and these outputs are organized in a structure and named to match the names of the components in the model.




!!! info "See Also"
    * [xolotl.show](../xolotl/#show)
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.transpile](../xolotl/#transpile)
    * [xolotl.compile](../xolotl/#compile)







-------

### loadobj

**Description**

A static method that overloads the built-in loadobj method. This sets 
local parameters, and is useful when xolotl objects are saved to
disk and loaded. 

!!! warning 
    Do not use this method. This method exists so that MATLAB knows how to load xolotl objects correctly





-------

### manipulate



**Syntax**

```matlab
x.manipulate();
x.manipulate('some*pattern')
x.manipulate({'parameter1','parameter2'})
x.manipulate({'parameter1','parameter2'},mirror_these)
x.manipulate(x.Compartment.Channel)
```

**Description**

`manipulate` is a method that allows you to manipulate some or all parameters in a model while visualizing its behavior.

- **`x.manipulate()`** manipulates all the parameters in a xolotl model. It wires up sliders to all parameters, and moving these sliders causes the model to integrate, and a plot to update.
- **`x.manipulate('some*pattern')`** creates sliders only for parameters specified by 'some*pattern'.
- **`x.manipulate({'parameter1','parameter2'})`** creates sliders only for the parameters specified in the cell array. Parameters should resolve to valid properties of cpplab objects in the tree.
- **`x.manipulate({'parameter1','parameter2'},mirror_these)`** creates sliders only for the parameters specified in the cell array. Parameters should resolve to valid properties of cpplab objects in the tree. In addition, parameters specified in mirror_these are also changed to reflect parameters changed in the first argument. 
- **`x.manipulate(x.Compartment.Channel)`** If manipulate is given one argument which is a channel in a compartment, then parameters of that channel can be manipulated, and an additional window will be drawn plotting the activation functions of that channel. 




!!! info "See Also"
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.show](../xolotl/#show)








-------

### manipulateConductance

This method is used to update the `xolotl` object
every time a slider is moved in the manipulate window.
This is used internally in `xolotl.manipulate`. You
should not need to use this by itself. 



!!! info "See Also"
    * [xolotl.manipulate](../xolotl/#manipulate)
    * [xolotl.manipulateEvaluate](../xolotl/#manipulateevaluate)







-------

### manipulateEvaluate

This method is used to update the `xolotl` object
every time a slider is moved in the manipulate window.
This is used internally in `xolotl.manipulate`. You
should not need to use this by itself.



!!! info "See Also"
    * [xolotl.manipulate](../xolotl/#manipulate)







-------

### manipulateEvaluateCond

This method is used to update the `xolotl` object
every time a slider is moved in the manipulate window.
This is used internally in `xolotl.manipulate`. You
should not need to use this by itself. 



!!! info "See Also"
    * [xolotl.manipulate](../xolotl/#manipulate)
    * [xolotl.manipulateEvaluate](../xolotl/#manipulateevaluate)



-------

### plot

**Syntax**

```matlab
x.plot()
x.plot('comp_name')
x.plot({'comp1','comp2'...})
```

** Description**

- **`x.plot`**  makes a plot of voltage and calcium time series of all
compartments. The default option is to color the voltage
traces by the dominant current at that point using
`contributingCurrents` and to also show the Calcium
concentration on the same plot.
- **`x.plot('comp_name')`** Plots voltage traces from only that compartment.
- **`x.plot({'comp1','comp2'...}))`** plots voltage traces from these compartments.


If you want to turn off the coloring, or to hide the
Calcium concentration, change your preference using:


```matlab
x.pref.plot_color = false;
x.pref.show_Ca = false;
```



!!! info "See Also"
    * [xolotl.manipulate](../xolotl/#manipulate)
    * [xolotl.contributingCurrents](../xolotl/#contributingcurrents)
    * [xolotl.currentscape](../xolotl/#currentscape)





-------

### plotgbars


**Syntax**

```matlab
x.plotgbars('compartment_name');
x.plotgbars(axes_handle,'compartment_name');
axes_handle = x.plotgbars(axes_handle,'compartment_name');
```

**Description**

Makes a stem plot of conductance densities in a given compartment.
If the first argument is a handle to a valid axis, plots will be made there.
If no axis handle is given, it will use `gca` instead.
This function can plot onto both Cartesian and polar axes.




!!! info "See Also"
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.show](../xolotl/#show)
    * [xolotl.manipulate](../xolotl/#manipulate)







-------

### porcupine 

**Syntax**

```matlab
x.porcupine()
data = x.porcupine
```

** Description**

- **`x.porcupine`**  makes a plot of voltage and inward and outward currents
in a polar plot, wrapped to the burst period of the neuron (assuming it bursts)
- **`data = x.porcupine`**  does not make a plot, but returns a data structure indicating
the direction and magnitude of the greatest contribution of each current



!!! info "See Also"
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.currentscape](../xolotl/#currentscape)





-------

### rebase

**Syntax**

```matlab
x.rebase()
```

**Description**

`rebase` is an internal method that configures some
house-keeping settings. rebase is called every
time a new xolotl object is created. rebase:

1. configures the `xolotl_folder` property
2. configures the `cpp_folder` property, which tells xolotl where its C++ files are located
3. calls the rebase method from the cpplab superclass.


If you move a xolotl object across computers (for example, by saving it to a file and loading it in a different computer), you must call `rebase` to link it to the C++ files it needs.




!!! info "See Also"
    * [cpplab.rebase](../cpplab/#rebase)







-------

### reset





**Syntax**

```matlab
x.reset()
x.reset('snap_name')
```

**Description**

Resets a xolotl object to some default state.

reset called without any arguments resets the model as best as it can -- voltages are set to -60 mV, Calcium in every compartment is set to the internal value, and the gating variables of every conductance are reset.

`reset` can also be called with a string argument, which is the name of a snapshot previously stored in the model object. Then, `reset` reconfigures the parameters of the model to match that snapshot. This is useful for working with a model, changing parameters, evolving it, and then coming back to where you started off from.

Here's an example:

```
% assuming a xolotl object is set up
x.integrate;
x.snapshot('base');
x.set('*gbar') = 1e-3; % turn off all conductances
x.integrate;
% now go back to original state
x.reset('base')
```

This method supports tab completion. You should be able to
press tab and get a list of snapshots that you want to
reset to.



!!! info "See Also"
    * [xolotl.snapshot](../xolotl/#snapshot)







-------

### rheobase


**Syntax**

```matlab
options = x.rheobase
I = x.rheobase('PropertyName', PropertyValue, ...)
I = x.rheobase(options)
```

**Description**

Finds the minimum injected current required to cause a xolotl model to spike.
The model is simulated with increasing amounts of constant injected current until one spike is elicited.
This minimum amount of current is called the rheobase.
The output `I` contains the current magnitude needed to cause the model to spike,

If called without arguments and one output, a struct
containing fields for all optional arguments, `options`,
is returned.

Otherwise, the first argument should be a xolotl object,
and the latter should be either name, value keyword pairs,
or a struct to specify options.
Options with a `NaN` value are ignored, and the default is used instead.

| Option Name | Default Value | Units |
| ----------- | ------------- | ----- |
| `I_min` | -0.2 | nA |
| `I_max` | 4 | nA |
| `SpikeThreshold` | 0 | mV |
| `nSpikes` | 1 | |
| `t_end` | 10e3 | ms |

`I_min` and `I_max` define the minimum and maximum of the tested injected current.
The `SpikeThreshold` defines when a spike is said to occur.
The voltage must cross this threshold in order for a spike to be counted.
You can change the number of spikes to search for via the `nSpikes` option.
This should be used in combination with the `t_end` option,
which defines how long the simulation is.
For example, to find the current at which your model spikes at 10 Hz,
you could set `t_end = 10e3` and `nSpikes = 10`.




!!! info "See Also"
    * [xtools.V2metrics](../xtools/#v2metrics)
    * [xtools.findNSpikes](../xtools/#findnspikes)
    * [xtools.findNSpikeTimes](../xtools/#findnspiketimes)
    * [xolotl.fI](../xolotl/#fi)




-------

### run_all_tests



**Syntax**

```matlab
xolotl.run_all_tests
```

**Description**

A static method that runs all tests (demos/examples)
in xolotl/examples. If you've just installed this,
it may be a good idea to run this to make sure everything works.

This method is called during testing, and only if all
tests pass is a release published.






-------

### setup

**Syntax**

```matlab
xolotl.setup
x.setup
```

**Description**

A static method that allows you to set up compilers
on some operating systems. You need to run this only
once. If xolotl works, there is no need to run this.




!!! info "See Also"
    * [xolotl.update](../xolotl/#update)
    * [xolotl.uninstall](../xolotl/#uninstall)






-------

### show

**Syntax**

```matlab
x.show('path/to/conductance/file')
x.show(x.Compartment)
x.show(x.Compartment.Conductance)
x.show({conductance1, conductance2...})
x.show(x.Compartment.Synapse)
```

This method displays activation functions and timescales of any conductance or synapse. If you are 
passing a conductance argument, subsequent calls to `show` will update the plot, 
plotting the new activation curves over the old ones, allowing you to compare different channels.





!!! info "See Also"
    * [xolotl.plot](../xolotl/#plot)
    * [xolotl.getGatingFunctions](../xolotl/#getgatingfunctions)







-------

### slice

**Syntax**

```matlab
% assuming there is a compartment called 'Dendrite'
x.slice('Dendrite',10)
```

**Description**

`slice` partitions a cylindrical compartment into N slices.

The compartment to be sliced must explicitly be a cylindrical
section, i.e., it must have a defined length and radius.
`slice` cuts the cylinder along the axis, and connects each
slice with `Axial` synapses. This object can then be treated
as a multi-compartment model, and `xolotl` will integrate
it using the Crank-Nicholson scheme reserved for multi-compartment models.




!!! info "See Also"
    * [xolotl.connect](../xolotl/#connect)





-------

### snapshot

**Syntax**

```matlab
x.snapshot('snap_name')
```

**Description**

Saves the current state of a ``xolotl`` object for future use.
Snapshots act as properties of the ``xolotl`` object, so can be reset to anytime,
including within other function and scopes.

!!! warning
    Creating two snapshots with the same name will overwrite the first.


**Example**

```
% assuming a xolotl object is set up
x.integrate;
x.snapshot('base');
x.set('*gbar') = 1e-3; % turn off all conductances
x.integrate;
% now go back to original state
x.reset('base')
```




!!! info "See Also"
    * [xolotl.reset](../xolotl/#reset)
    * [How to: save configurations and use snapshots](https://xolotl.readthedocs.io/en/master/how-to/snapshots/)






-------

### transpile


**Syntax**

```matlab
x.transpile;
```

**Description**

Generate a C++ file that constructs the model,
integrates it, and moves parameters and data from
MATLAB to C++ and back.



!!! warning
    Manual transpiling is discouraged. xolotl will automatically transpile code for you when needed.




!!! info "See Also"
    * [xolotl.compile](../xolotl/#compile)
    * [xolotl.viewCode](../xolotl/#viewcode)







-------

### transpileCore

**Syntax**

```matlab
x.transpileCore(in_file,out_file)
```

**Description**

method that writes C++ bridge code to set up
and integrate your model based on the
objects configured in the xolotl tree. This is
internally called by xolotl.transpile()

Do not call this method. It is not meant
to be user accessible.



!!! info "See Also"
    * [xolotl.transpile](../xolotl/#transpile)






-------

### uninstall

**Syntax**

```matlab
xolotl.uninstall
```

**Description**

Static method of xolotl that uninstalls the currently
installed version of xolotl. If xolotl was installed
as a MATLAB toolbox, this method deletes the toolbox
and removes the toolbox from the path.




!!! info "See Also"
    * [xolotl.update](../xolotl/#update)






-------

### update

**Syntax**

```matlab
xolotl.update
```

**Description**

Static method of xolotl that updates the currently
installed version of xolotl to the latest version.
This method needs an active internet connection to 
work. If xolotl was installed using git, it will
attempt to use git to update. If xolotl was installed
as a MATLAB toolbox, it will download the latest 
version of the toolbox and will attempt to replace the
installed version with the latest version.

!!! warning
    Updating xolotl if installed as a MATLAB toolbox will overwrite preferences and delete compiled binaries. 



!!! info "See Also"
    * [xolotl.uninstall](../xolotl/#uninstall)




-------

### viewCode

**Syntax**

```matlab
x.viewCode;
```


**Description**

view the C++ code generated by `xolotl.transpile()`
that constructs the model and integrates it






!!! info "See Also"
    * [xolotl.transpile](../xolotl/#transpile)
    * [xolotl.compile](../xolotl/#compile)







