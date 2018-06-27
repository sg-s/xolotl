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



Test coverage
--------------

``compile`` is tested in: 




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



Test coverage
--------------

``getGatingFunctions`` is tested in: 




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




