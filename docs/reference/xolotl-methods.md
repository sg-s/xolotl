

-------
## add


**Syntax**

```matlab
x.add('compartment','comp_name')
x.add(compartment,'comp_name')
x.add('compartment','comp_name',...)
```

**Description**

Adds a `cpplab` object to a `xolotl` object. The `add` method is the most important way you construct models. 

- **`x.add('compartment','comp_name')`** adds a compartment to the xolotl object and names it `comp_name`.

- **`x.add(compartment,'comp_name')`** adds a compartment object (a cpplab object) to the xolotl object `x` and names it `comp_name`. Note that compartment is a cpplab object sourced from the `compartment.hpp` C++ file, and can contain children and be extensively modified. 

- **`x.add('compartment','comp_name',...)`** adds a compartment to the xolotl object and names it `comp_name`. The compartment is then additionally configured using the parameters specified using Name Value syntax. 

**Technical Details**

`xolotl.add` checks that the compartment being added has a legal name using `checkCompartmentName`. If so, it calls the `add` method in the `cpplab` superclass. 

!!! info "See Also"
    * [cpplab.add](https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#add)
    * [xolotl.checkCompartmentName](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#checkcompartmentname)








-------
## benchmark

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
## checkCompartmentName

**Syntax**

```matlab
ok = checkCompartmentName(self,comp_name)
```

**Description**

`checkCompartmentName` is used internally by `xolotl` 
to verify that the compartment name you are using is valid and legal.  This method is called every time you add a compartment
to a `xolotl` object. 

!!! warning 
    Do not use `checkCompartmentName`, as it may be removed in a future release.

!!! info "See Also"
    * [xolotl.add](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#add)








-------
## checkTree

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
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)
    * [xolotl.compile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#compile)








-------
## cleanup

**Syntax**

```matlab
xolotl.cleanup
x.cleanup
```

**Description**

A static method that cleans up all transpiled ``C++`` and compiled binary files.

!!! warning 
    Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 








-------
## compile

**Syntax**

```matlab
x.skip_hash = true;
```

**Description**

compiles a executable binary form a transpiled ``C++`` file. 
These are stored in your ``xolotl`` directory. ``xolotl`` 
automatically compiles when t needs to. You can turn this 





-------
## connect


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








-------
## contributingCurrents

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
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)








-------
## copy

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
    * [cpplab.copy()](https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#copy())

    * [How to copy models](https://xolotl.readthedocs.io/en/master/how-to/copy-models/)








-------
## fI

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








-------
## getGatingFunctions


**Syntax**

```matlab
[m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance)
```

**Description**

static method of `xolotl` that returns function handles
that represent the gating and activation functions of a
particular conductance. 


`conductance` is a string that specifies a
conductance C++ header file. The outputs are function 
handles that can be evaluated independently. This method
is used internally in `xolotl.show()`

!!! info "See Also"
    * [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)









-------
## go_to_examples


**Syntax**

```matlab
xolotl.go_to_examples
```

**Description**

A static method that goes to the folder that contains xolotl examples. 

!!! info "See Also"
    * [xolotl.run_all_tests](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#run_all_tests)









-------
## integrate

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
    * [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)
    * [xolotl.compile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#compile)










-------
## make_bursting_neuron

**Syntax**

```matlab
x = xolotl.make_bursting_neuron('liu');
x = xolotl.make_bursting_neuron('prinz');
```

**Description**

makes a single-compartment neuron that bursts, using
channels from Liu et al. or Prinz et al. 










-------
## manipulate



**Syntax**

```matlab
x.manipulate();
x.manipulate('some*pattern')
x.manipulate({'parameter1','parameter2'})
```

**Description**

`manipulate` is a method that allows you to manipulate some or all parameters in a model while visualizing its behavior. 

- **`x.manipulate()`** manipulates all the parameters in a xolotl model. It wires up sliders to all parameters, and moving these sliders causes the model to integrate, and a plot to update. 
- **`x.manipulate('some*pattern')`** creates sliders only for parameters specified by 'some*pattern'. 
- **`x.manipulate({'parameter1','parameter2'})`** creates sliders only for the parameters specified in the cell array. Parameters should resolve to valid properties of cpplab objects in the tree. 


!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)










-------
## manipulateEvaluate

This method is used to update the `xolotl` object 
every time a slider is moved in the manipulate window. 
This is used internally in `xolotl.manipulate`. You 
should not need to use this by itself. 

!!! info "See Also"
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)










-------
## plot

**Syntax**

```matlab
x.plot()
```

** Description**

`x.plot` makes a plot of voltage and calcium time series of all 
compartments. The default option is to color the voltage
traces by the dominant current at that point using  
`contributingCurrents` and to also show the Calcium 
concentration on the same plot. 


If you want to turn off the coloring, or to hide the 
Calcium concentration, change your preference using:


```matlab
x.pref.plot_color = false;
x.pref.show_Ca = false;
```

!!! info "See Also"
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)
    * [xolotl.contributingCurrents](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#contributingcurrents)









-------
## plotgbars


**Syntax**

```matlab
x.plotgbars('compartment_name');
x.plotgbars(axes_handle,'compartment_name');
```

**Description**

Makes a stem plot of conductance densities in a given compartment. If the first argument is a handle to a valid axis, plots will be made there. 


!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)










-------
## rebase

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
    * [cpplab.rebase()](https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#rebase())









-------
## reset





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
	
!!! info "See Also"
    * [xolotl.snapshot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#snapshot)









-------
## run_all_tests



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
## setup

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
    * [xolotl.update](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#update)
    * [xolotl.uninstall](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#uninstall)









-------
## show

**Syntax**

```matlab
xolotl.show('path/to/conductance/file')
```


This method displays activation functions and timescales of any conductance. Subsequent calls to `show` will update the plot, plotting the new activation curves over the old ones, allowing you to compare different channels. 


!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.getGatingFunctions](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#getgatingfunctions)









-------
## slice

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
    * [xolotl.connect](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#connect)








-------
## snapshot

**Syntax**

```matlab
x.snapshot('snap_name')
```

**Description**

Saves the current state of a ``xolotl`` object for future use. 


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
    * [xolotl.reset](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#reset)
    * [How to: save configurations and use snapshots](https://xolotl.readthedocs.io/en/master/how-to/snapshots/)









-------
## transpile


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
    * [xolotl.compile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#compile)
    * [xolotl.viewCode](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#viewcode)










-------
## transpileCore

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
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)








-------
## uninstall

**Syntax**

```matlab
xolotl.uninstall
x.uninstall
```


**Description**

A static method that uninstalls your installation 
of xolotl in place. If you installed using git, 
`xolotl` will attempt to use git to uninstall 
itself. 



!!! info "See Also"
    * [xolotl.update](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#update)








-------
## update

**Syntax**


```matlab
xolotl.update()
```


**Description**

A static method that updates your installation of 
`xolotl` in place. If you installed using git, 
`xolotl` will attempt to use git to update itself.



!!! info "See Also"
    * [xolotl.uninstall](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#uninstall)








-------
## viewCode

**Syntax**

```matlab
x.viewCode;
```


**Description**

view the C++ code generated by `xolotl.transpile()` 
that constructs the model and integrates it




!!! info "See Also"
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)






