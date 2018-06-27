.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _methods:

Methods
=======

This page lists the methods of the ``xolotl`` class in ``MATLAB``. This can serve as a reference for advanced usage. 

In the rest of this documentation we will assume a ``xolotl`` object named ``x`` that can be created using

  x = xolotl;

.. hint::

  You can list all the methods of ``xolotl`` object ``x`` by ::

    x.methods;
.. _add:

add
^^^

adds a ``cpplab`` object to a ``xolotl`` object.

The add method is the most important way you construct models. Usage ::

	x.add(compartment,'comp_name')
	x.add('compartment','comp_name')
	x.add('compartment','comp_name',...)

There are two primary ways of using ``add``. The first is to first construct a ``cpplab`` object (here called AB), and then add it to the ``xolotl`` object using ``x.add(AB,'AB')``. ``xolotl`` requires that every compartment is named, and the name has to be specified as a string argument. 

.. warning::

Some read-only properties in a xolotl object may not be copied over. 





Test coverage
--------------

``add`` is tested in: 

- `test_bursting_neuron.m <https://github.com/sg-s/xolotl/blob/master/tests/test_bursting_neuron.m>`_ 
- `test_clamp.m <https://github.com/sg-s/xolotl/blob/master/tests/test_clamp.m>`_ 
- `test_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/test_fI.m>`_ 
- `test_integral_control.m <https://github.com/sg-s/xolotl/blob/master/tests/test_integral_control.m>`_ 
- `test_stg.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg.m>`_ 
- `test_stg_temperature.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg_temperature.m>`_ 



.. _benchmark:

benchmark
^^^^^^^^^

performs a quick benchmarking of a given ``xolotl`` model. ``benchmark`` irst varies the simulation time step, and measures how quickly the model ntegrates. It then varies ``t_end``, and measures how fast it integrates at a fixed ``sim_dt``. Usage ::

    x.benchmark;






Test coverage
--------------

``benchmark`` is tested in: 




.. _checkCompartmentName:

checkCompartmentName
^^^^^^^^^^^^^^^^^^^^

is used internally by ``xolotl`` to verify that the compartment name you are using is valid and legal. This method is called every time you add a compartment to a ``xolotl`` object. Usage ::

   ok = checkCompartmentName(self,comp_name)

See Also
--------


 - `add <https://xolotl.readthedocs.io/en/latest/auto_methods.html#add>`_ 





Test coverage
--------------

``checkCompartmentName`` is tested in: 




.. _cleanup:

cleanup
^^^^^^^
A static method that cleans up all transpiled ``C++`` and compiled binary files. Usage ::

   xolotl.cleanup
   x.cleanup

Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 





Test coverage
--------------

``cleanup`` is tested in: 

- `run_all_tests.m <https://github.com/sg-s/xolotl/blob/master/tests/run_all_tests.m>`_ 



.. _compile:

compile
^^^^^^^

compiles a executable binary form a transpiled ``C++`` file. These are stored in your ``xolotl`` directory. ``xolotl`` automatically compiles when t needs to. You can turn this functionality off by setting ::

    x.skip_hash = true;

In addition, creating a ``xolotl`` object through a function call does not utomatically hash and compile. In this case, you should use ``x.md5hash``.

.. warning::

If you turn hashing off, ``xolotl`` might not compile


See Also:
---------


 - `transpile <https://xolotl.readthedocs.io/en/latest/auto_methods.html#transpile>`_ 

 - `cleanup <https://xolotl.readthedocs.io/en/latest/auto_methods.html#cleanup>`_ 





Test coverage
--------------

``compile`` is tested in: 




.. _connect:

connect
^^^^^^^

Connects two compartments with a synapse. The basic syntax is ::

   x.connect('Comp1', 'Comp2', 'SynapseType', ...)

The first two arguments are the presynaptic and postsynaptic compartment ames. For example ::

    % connects two different neurons with an electrical synapse
    x.connect('AB', 'LP')

Axial synapses are a special type of electrical synapse that are created etween spatially-discrete compartments in a morphological structure. Electrical and axial synapses differ in how they are integrated (see ayan & Abbott 2001, Ch. 5-6).

``connect`` defaults to an axial synapse when the type of synapse is not pecified and either compartment has a defined ``tree_idx`` (which dentifies the compartment as a part of a multi-compartment neuron model). Otherwise, the created synapse is electrical. ::

   % create an (electrical or axial) synapse between AB and LP with gbar f NaN
   x.connect('AB', 'LP')
   % create an (electrical or axial) synapse between AB and LP with gbar f 10
   x.connect('AB', 'LP', 10)


The most common way to produce a synapse is to pass the synapse type and hen any properties. This is used to create chemical synapses. For example, o add a glutamatergic synapse (from Prinz *et al.* 2004) between ``AB`` nd ``LP`` with a maximal conductance of 100: ::

   x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)


Synapses can also be connected by passing a ``cpplab`` object to the `connect`` method ::


    % create a synapse using the cpplab object 'syn_cpplab' 
    x.connect('AB', 'LP', syn_cpplab)


The following properties can be specified

======================= ================
Name                    PropertyName
Maximal conductance     ``gbar``
Reversal potential      ``E``
Activation variable     ``s``
======================= ================





Test coverage
--------------

``connect`` is tested in: 

- `test_stg.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg.m>`_ 



.. _contributingCurrents:

contributingCurrents
^^^^^^^^^^^^^^^^^^^^

calculates the contributions of each current at every point in a voltage race. This is used internally in ``xolotl.plot`` to color voltage traces. yntax ::

    curr_index = contributingCurrents(V, I)

where V is a vector of voltages, I is the corresponding matrix of currents 

See Also
--------


 - `plot <https://xolotl.readthedocs.io/en/latest/auto_methods.html#plot>`_ 

 - `manipulate <https://xolotl.readthedocs.io/en/latest/auto_methods.html#manipulate>`_ 





Test coverage
--------------

``contributingCurrents`` is tested in: 




.. _copy:

copy
^^^^

copies a xolotl object. ``copy`` creates an identical copy of a xolotl object that can be manipulated seperately. Both copies will use the same binary to integrate, unless you add a new component to one of them. Syntax ::

    x2 = copy(x);

.. warning::

Some read-only properties in a xolotl object may not be copied over. 


.. warning::

Do not make vectors of ``xolotl`` objects, as it may lead to undefined behavior. 





Test coverage
--------------

``copy`` is tested in: 




.. _findNSpikes:

findNSpikes
^^^^^^^^^^^

static method of ``xolotl`` that computes the number of spikes in a voltage trace. Example use ::

   f = findNSpikes(V);
   f = findNSpikes(V, on_off_thresh)

``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0. 

See Also
--------


 - `findNSpikeTimes <https://xolotl.readthedocs.io/en/latest/auto_methods.html#findnspiketimes>`_ 







Test coverage
--------------

``findNSpikes`` is tested in: 




.. _findNSpikeTimes:

findNSpikeTimes
^^^^^^^^^^^^^^^

static method of ``xolotl`` that returns a vector of spike times from a voltage trace. Spikes are defined as voltage crossings across a threshold. Example use ::

   spiketimes = findNSpikeTimes(V,n_spikes,on_off_thresh);


``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0. ``n_spikes`` is the number of spikes it should look for, and ``spiketimes`` will always be a vector ``n_spikes`` elments long. 

See Also
--------


 - `findNSpikes <https://xolotl.readthedocs.io/en/latest/auto_methods.html#findnspikes>`_ 








Test coverage
--------------

``findNSpikeTimes`` is tested in: 




.. _getGatingFunctions:

getGatingFunctions
^^^^^^^^^^^^^^^^^^

static method of ``xolotl`` that returns function handles that represent the gating and activation functions of a particular conductance. Example use ::

   [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance)


where ``conductance`` is a string that specifies a conductance C++ header file. The outputs are function handles that can be evaluated independently. This method is used internally in ``xolotl.show()``

See Also
--------


 - `show <https://xolotl.readthedocs.io/en/latest/auto_methods.html#show>`_ 






Test coverage
--------------

``getGatingFunctions`` is tested in: 




.. _integrate:

integrate
^^^^^^^^^

integrates a ``xolotl`` model. Usage ::

   V = x.integrate;
   I_clamp = x.integrate;
   [V, Ca] = x.integrate;
   [V, Ca, cont_state] = x.integrate;
   [V, Ca, cont_state, I] = x.integrate;
   [V, Ca, cont_state, I, syn_state] = x.integrate;


``integrate`` will return different outputs as show above. Unless you need every output, it is recommended to skip it, as it makes the integration faster (and reduces the memory footprint). 

Explanation of outputs
----------------------

- ``V`` Voltage trace of every compartment. A matrix of size (nsteps, n_comps)
- ``I_clamp`` also returned in the first argument, this is the clamping current when a compartment is being voltage clamped. This can be inter-leaved with the voltage of other, non-clamped compartments. 
- ``Ca`` Calcium concentration in every cell and the corresponding ``E_Ca`` (reversal potential of Calcium). A matrix of size (nsteps, n_comps)
- ``cont_state`` a matrix representing every dimension of every controller in the tree. This matrix has size (nsteps, NC), where NC depends on the precise controllers used, and is automatically determined. 
- ``I`` the currents of every ion channel type in the model. This is a matrix of size (nsteps, n_cond)








Test coverage
--------------

``integrate`` is tested in: 

- `custom_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/custom_fI.m>`_ 
- `test_bursting_neuron.m <https://github.com/sg-s/xolotl/blob/master/tests/test_bursting_neuron.m>`_ 
- `test_clamp.m <https://github.com/sg-s/xolotl/blob/master/tests/test_clamp.m>`_ 
- `test_integral_control.m <https://github.com/sg-s/xolotl/blob/master/tests/test_integral_control.m>`_ 
- `test_stg.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg.m>`_ 
- `test_stg_temperature.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg_temperature.m>`_ 



.. _manipulate:

manipulate
^^^^^^^^^^

method that allows you to manipulate some or all parameters in a model hile visualizing its behaviour. Usage ::

   x.manipulate();
   x.manipulate('some*pattern')
   x.manipulate({'parameter1','parameter2'})

The simplest way to use ``manipulate`` is to simply call it with no arguments. By default, all the parameters are linked to sliders that you can play with. In models with a large number of parameters, this can get messy. You can selectively only manipualte some parameters whose names match a pattern using ``x.manipulate('some*pattern')``








Test coverage
--------------

``manipulate`` is tested in: 

- `custom_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/custom_fI.m>`_ 
- `test_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/test_fI.m>`_ 



.. _manipulateEvaluate:

manipulateEvaluate
^^^^^^^^^^^^^^^^^^

This method is used to update the ``xolotl`` object every time a slider is moved in the manipulate window. This is used internally in ``xolotl.manipulate``. You should not need to use this by itself. 

See Also
--------


 - `manipulate <https://xolotl.readthedocs.io/en/latest/auto_methods.html#manipulate>`_ 







Test coverage
--------------

``manipulateEvaluate`` is tested in: 




.. _plot:

plot
^^^^

Makes a plot of voltage and calcium time series of all compartments. The default option is to color the voltage traces by the dominant current at that point using  ``contributingCurrents``. Usage ::

   x.plot()

See Also
--------


 - `manipulate <https://xolotl.readthedocs.io/en/latest/auto_methods.html#manipulate>`_ 

 - `contributingCurrents <https://xolotl.readthedocs.io/en/latest/auto_methods.html#contributingcurrents>`_ 






Test coverage
--------------

``plot`` is tested in: 

- `custom_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/custom_fI.m>`_ 
- `test_bursting_neuron.m <https://github.com/sg-s/xolotl/blob/master/tests/test_bursting_neuron.m>`_ 
- `test_clamp.m <https://github.com/sg-s/xolotl/blob/master/tests/test_clamp.m>`_ 
- `test_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/test_fI.m>`_ 
- `test_integral_control.m <https://github.com/sg-s/xolotl/blob/master/tests/test_integral_control.m>`_ 
- `test_stg.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg.m>`_ 
- `test_stg_temperature.m <https://github.com/sg-s/xolotl/blob/master/tests/test_stg_temperature.m>`_ 



.. _rebase:

rebase
^^^^^^

Configures some internal house-keeping settings. This is called every time a new object is created. You probably don't ever have to use this, unless you copy ``xolotl`` objects across computers with different file systems or operating systems. Usage ::

   x.rebase()






Test coverage
--------------

``rebase`` is tested in: 




.. _reset:

reset
^^^^^

Resets a xolotl object to some default state. Usage ::

   x.reset()
   x.reset('snap_name')

reset called without any arguments resets the model as best as it can -- voltages are set to -60 mV, Calcium in every compartment is set to the internal value, and the gating variables of every conductance are reset. 

``reset`` can also be called with a string argument, which is the name of a snapshot previously stored in the model object. Then, ``reset`` reconfigures the parameters of the model to match that snapshot. This is useful for working with a model, changing parameters, evolving it, and then coming back to where you started off from. 

Example
-------

	% assuming a xolotl object is set up
	x.integrate;
	x.snapshot('base');
	x.set('*gbar') = 1e-3; % turn off all conductances
	x.integrate;
	% now go back to original state
	x.reset('base')

	

See Also
--------


 - `snapshot <https://xolotl.readthedocs.io/en/latest/auto_methods.html#snapshot>`_ 






Test coverage
--------------

``reset`` is tested in: 




.. _show:

show
^^^^^

shows activation functions and timescales of any conductance. Usage ::

   x.show('cond_name')

'cond_name' must be a string that resolves to a valid C++ file that describes a conductance. 

Example
-------

	% compare some channels from the Prinz et al. paper
    xolotl.show('prinz/NaV')
    xolotl.show('prinz/Kd')
    xolotl.show('prinz/KCa')

	

See Also
--------


 - `plot <https://xolotl.readthedocs.io/en/latest/auto_methods.html#plot>`_ 






Test coverage
--------------

``show`` is tested in: 

- `test_fI.m <https://github.com/sg-s/xolotl/blob/master/tests/test_fI.m>`_ 



.. _slice:

slice
^^^^^

``slice`` partitions a cylindrical compartment into N slices.  Usage ::

   x.slice('comp_name',N)

The compartment to be sliced must explicitly be a cylindrical section, i.e., it must have a defined length and radius. ``slice`` cuts the cylinder along the axis, and connects each slice with ``Axial`` synapses. This object can then be treated as a multi-compartment model, and ``xolotl`` will integrate it using the Crank-Nicholson scheme reserved for multi-compartment models. 


Example
-------

	% assuming there is a compartment called 'Dendrite'
    xolotl.slice('Dendrite',10)
	

See Also
--------


 - `connect <https://xolotl.readthedocs.io/en/latest/auto_methods.html#connect>`_ 





Test coverage
--------------

``slice`` is tested in: 




.. _snapshot:

snapshot
^^^^^^^^

Saves the current state of a ``xolotl`` object for future use. Usage ::

   x.snapshot('snap_name')


.. warning::

Creating two snapshots with the same name will overwrite the first. 


Example
-------

    % assuming a xolotl object is set up
    x.integrate;
    x.snapshot('base');
    x.set('*gbar') = 1e-3; % turn off all conductances
	x.integrate;

	% now go back to original state
	x.reset('base')

	

See Also
--------


 - `reset <https://xolotl.readthedocs.io/en/latest/auto_methods.html#reset>`_ 






Test coverage
--------------

``snapshot`` is tested in: 




.. _transpile:

transpile
^^^^^^^^^

Generate a C++ file that constructs the model, integrates it, and moves parameters and data from MATLAB to C++ and back. Usage ::

   x.transpile;


.. warning::

``transpile`` assumes that your ``xolotl`` object has a valid hash. Empty hashes will throw an error. 


Example
-------

    % assuming a xolotl object is set up
    x.transpile;

    % now view the transpiled code
    x.viewCode;

	

See Also
--------


 - `compile <https://xolotl.readthedocs.io/en/latest/auto_methods.html#compile>`_ 

 - `viewCode <https://xolotl.readthedocs.io/en/latest/auto_methods.html#viewcode>`_ 







Test coverage
--------------

``transpile`` is tested in: 




.. _viewCode:

viewCode
^^^^^^^^

view the C++ code generated by xolotl.transpile that constructs the model and integrates it ::

     x.viewCode;

See Also:
---------


 - `transpile <https://xolotl.readthedocs.io/en/latest/auto_methods.html#transpile>`_ 





Test coverage
--------------

``viewCode`` is tested in: 




