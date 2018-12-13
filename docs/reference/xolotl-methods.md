

# add

adds a `cpplab` object to a `xolotl` object.

The add method is the most important way you construct models. 

Usage:

```matlab
x.add(compartment,'comp_name')
x.add('compartment','comp_name')
x.add('compartment','comp_name',...)
```

There are two primary ways of using `add`. The first is to 
first construct a `cpplab` object (here called AB), and then
 add it to the `xolotl` object using `x.add(AB,'AB')`. 
`xolotl` requires that every compartment is named, and 
the name has to be specified as a string argument. 









# benchmark

performs a quick benchmarking of a given `xolotl` model. 
`benchmark` first varies the simulation time step, and 
measures how quickly the model integrates. It then 
varies `t_end`, and measures how fast it integrates 
at a fixed `sim_dt`. 

Usage:

```
x.benchmark;
```








# checkCompartmentName

is used internally by `xolotl` to verify that the
compartment name you are using is valid and legal. 
This method is called every time you add a compartment
to a `xolotl` object. 

Usage:

```matlab
ok = checkCompartmentName(self,comp_name)
```

!!! info "See Also"
    * [xolotl.add](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#add)








# cleanup

A static method that cleans up all transpiled ``C++`` and compiled binary files. Usage:

```
xolotl.cleanup
x.cleanup
```

Use of this method will trigger a warning every time it 
is called. You do not need to use this in normal use, 
but can call this to force a recompile, or to delete old 
and unused binaries. 








# compile

compiles a executable binary form a transpiled ``C++`` file. 
These are stored in your ``xolotl`` directory. ``xolotl`` 
automatically compiles when t needs to. You can turn this 





# connect

Connects two compartments with a synapse. The basic syntax is ::


```
x.connect('Comp1', 'Comp2', 'SynapseType', ...)
```

The first two arguments are the presynaptic and postsynaptic compartment names. For example ::

```
% connects two different neurons with an electrical synapse
x.connect('AB', 'LP')
```

Axial synapses are a special type of electrical synapse that are created between spatially-discrete compartments in a morphological structure. Electrical and axial synapses differ in how they are integrated (see Dayan & Abbott 2001, Ch. 5-6).

`connect` defaults to an axial synapse when the type of synapse is not specified and either compartment has a defined `tree_idx` (which identifies the compartment as a part of a multi-compartment neuron model). Otherwise, the created synapse is electrical. ::


```
% create an (electrical or axial) synapse between AB and LP with gbar f NaN
x.connect('AB', 'LP')
% create an (electrical or axial) synapse between AB and LP with gbar f 10
x.connect('AB', 'LP', 10)
```

The most common way to produce a synapse is to pass the synapse type and hen any properties. This is used to create chemical synapses. For example, o add a glutamatergic synapse (from Prinz *et al.* 2004) between `AB` and `LP` with a maximal conductance of 100: ::

```
x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)
```

Synapses can also be connected by passing a `cpplab` object to the `connect` method:

```
% create a synapse using the cpplab object 'syn_cpplab' 
x.connect('AB', 'LP', syn_cpplab)
```

The following properties can be specified

| Name                 |   PropertyName |
|----------------------| -----------| 
| Maximal conductance  |  `gmax`|
| Reversal potential   |  `E`|
| Activation variable  |  `s`|








# contributingCurrents

This static method calculates the contributions of each
current at every point in a voltage race. This is used 
internally in ``xolotl.plot`` to color voltage traces. 
The syntax is:


```
curr_index = xolotl.contributingCurrents(V, I)
```

where V is a vector of voltages, I is the corresponding matrix of currents 

!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)








# copy

copies a xolotl object. ``copy`` creates an identical copy of a xolotl object that can be manipulated seperately. Both copies will use the same binary to integrate, unless you add a new component to one of them. 

Syntax:

```
x2 = copy(x);
```

!!! warning
    Some read-only properties in a xolotl object may not be copied over. 


!!! warning
    Do not make vectors of ``xolotl`` objects, as it may lead to undefined behavior. 








# fI

This method computes the f-I (firing-rate vs current) 
curve of a single compartment model. 

Usage:

```
x.fI(I_min, I_max, n_steps)
```








# getGatingFunctions


static method of `xolotl` that returns function handles
that represent the gating and activation functions of a
particular conductance. 

Example use:


```
[m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance)
```


where `conductance` is a string that specifies a
conductance C++ header file. The outputs are function 
handles that can be evaluated independently. This method
is used internally in `xolotl.show()`

!!! info "See Also"
    * [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)









# go_to_examples

A static method that goes to the folder that contains xolotl examples. 

Usage:

```
xolotl.go_to_examples
```

!!! info "See Also"
    * [xolotl.run_all_tests](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#run_all_tests)









# integrate

integrates a ``xolotl`` model. 

Usage:


```
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


``integrate`` will return different outputs as shown above. 
Unless you need every output, it is recommended to skip it, 
as it makes the integration faster (and reduces the memory footprint). 

### Explanation of outputs


- ``V`` Voltage trace of every compartment. A matrix of size (nsteps, n_comps)
- ``I_clamp`` also returned in the first argument, this is the clamping current when a compartment is being voltage clamped. This can be inter-leaved with the voltage of other, non-clamped compartments. 
- ``Ca`` Calcium concentration in every cell and the corresponding ``E_Ca`` (reversal potential of Calcium). A matrix of size (nsteps, n_comps)
- ``mech_state`` a matrix representing every dimension of every mechanism in the tree. This matrix has size (nsteps, NC), where NC depends on the precise controllers used, and is automatically determined. 
- ``I`` the currents of every ion channel type in the model. This is a matrix of size (nsteps, n_cond)

!!! info "See Also"
    * [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)
    * [xolotl.compile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#compile)










# manipulate

method that allows you to manipulate some or all parameters in a model hile visualizing its behaviour. 

Usage:

```
x.manipulate();
x.manipulate('some*pattern')
x.manipulate({'parameter1','parameter2'})
```

The simplest way to use ``manipulate`` is to simply call 
it with no arguments. By default, all the parameters are 
linked to sliders that you can play with. In models with 
a large number of parameters, this can get messy. You can 
selectively only manipualte some parameters whose names 
match a pattern using ``x.manipulate('some*pattern')``

!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)










# manipulateEvaluate

This method is used to update the `xolotl` object 
every time a slider is moved in the manipulate window. 
This is used internally in `xolotl.manipulate`. You 
should not need to use this by itself. 

!!! info "See Also"
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)










# plot


Makes a plot of voltage and calcium time series of all 
compartments. The default option is to color the voltage
traces by the dominant current at that point using  
`contributingCurrents` and to also show the Calcium 
concentration on the same plot. 

Usage:

```
x.plot()
```

If you want to turn off the colouring, or to hide the 
Calcium concentration, change your preference using:


```
setpref('xolotl','plot_color',false)
setpref('xolotl','show_Ca',false)
```

!!! info "See Also"
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)
    * [xolotl.contributingCurrents](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#contributingcurrents)









# plotgbars

makes a stem plot of conductance densities in a given compartment. 


Usage:

```
x.plotgbars('compartment_name');
x.plotgbars(axes_handle,'compartment_name');
```

!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)
    * [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)










# rebase

rebase is an internal method that configures some 
house-keeping settings. rebase is called every 
time a new xolotl object is created. rebase:

1. configures the `xolotl_folder` property
2. configures the `cpp_folder` property, which tells xolotl where its C++ files are located
3. configures the `OS_binary_ext`, which is the extention of compiled binaries and is platform dependent. 
4. calls the rebase method from the cpplab superclass.

Usage:

```
x.rebase()
```

!!! info "See Also"
    ->cpplab.rebase()









# reset

Resets a xolotl object to some default state. 

Usage:

```
x.reset()
x.reset('snap_name')
```

reset called without any arguments resets the model as best as it can -- voltages are set to -60 mV, Calcium in every compartment is set to the internal value, and the gating variables of every conductance are reset. 

``reset`` can also be called with a string argument, which is the name of a snapshot previously stored in the model object. Then, ``reset`` reconfigures the parameters of the model to match that snapshot. This is useful for working with a model, changing parameters, evolving it, and then coming back to where you started off from. 

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









# run_all_tests

A static method that runs all tests (demos/examples)
in xolotl/examples. If you've just installed this,
it may be a good idea to run this using:

```
xolotl.run_all_tests
```

to make sure everything works. 

This method is called during testing, and only if all 
tests pass is a release published. 









# setup

A static method that allows you to set up compilers 
on some operating systems. You need to run this only 
once. If xolotl works, there is no need to run this. 

Usage:

```
xolotl.setup
x.setup
```

!!! info "See Also"
    * [xolotl.update](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#update)
    * [xolotl.uninstall](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#uninstall)









# show

shows activation functions and timescales of any conductance. 


Usage example:

```
xolotl.show('prinz/NaV')
xolotl.show('prinz/Kd')
xolotl.show('prinz/KCa')
```


!!! info "See Also"
    * [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
    * [xolotl.getGatingFunctions](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#getgatingfunctions)









# slice

`slice` partitions a cylindrical compartment into N slices.  


Usage:

```
% assuming there is a compartment called 'Dendrite'
x.slice('Dendrite',10)
```

The compartment to be sliced must explicitly be a cylindrical 
section, i.e., it must have a defined length and radius. 
`slice` cuts the cylinder along the axis, and connects each 
slice with `Axial` synapses. This object can then be treated 
as a multi-compartment model, and `xolotl` will integrate 
it using the Crank-Nicholson scheme reserved for multi-compartment models. 


!!! info "See Also"
    * [xolotl.connect](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#connect)








# snapshot

Saves the current state of a ``xolotl`` object for future use. Usage ::


```
x.snapshot('snap_name')
```

!!! warning
    Creating two snapshots with the same name will overwrite the first. 


Usage:

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









# transpile


Generate a C++ file that constructs the model, 
integrates it, and moves parameters and data from 
MATLAB to C++ and back. 


Usage:


```
x.transpile;
```

!!! warning 
    Manually transpiling is discouraged. xolotl will automatically transpile code for you when needed. 
	

!!! info "See Also"
    * [xolotl.compile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#compile)
    * [xolotl.viewCode](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#viewcode)










# transpileCore

method that writes C++ bridge code to set up
and integrate your model based on the 
objects configured in the xolotl tree. This is 
internally called by xolotl.transpile()

Do not call this method. It is not meant 
to be user accessible. 

!!! info "See Also"
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)








# uninstall

A static method that uninstalls your installation 
of xolotl in place. If you installed using git, 
`xolotl` will attempt to use git to uninstall 
itself. 

Usage:

```matlab
xolotl.uninstall
x.uninstall
```

!!! info "See Also"
    * [xolotl.update](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#update)








# update

A static method that updates your installation of 
`xolotl` in place. If you installed using git, 
`xolotl` will attempt to use git to update itself.

Usage:


```matlab
xolotl.update()
```

!!! info "See Also"
    * [xolotl.uninstall](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#uninstall)








# viewCode

view the C++ code generated by `xolotl.transpile()` 
that constructs the model and integrates it


Usage:

```
x.viewCode;
```

!!! info "See Also"
    * [xolotl.transpile](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#transpile)






