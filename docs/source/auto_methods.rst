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
adds a cpplab object to a xolotl object.
The add method is the most important way you construct models. Usage ::
  x.add(compartment,'comp_name')
  x.add('compartment','comp_name')
  x.add('compartment','comp_name',...)
There are two primary ways of using ``add``. The first is to first construct a ``cpplab`` object (here called AB), and then add it to the xolotl object using x.add(AB,'AB'). xolotl requires that every compartment is named, and the name has to be specified as a string argument. 

.. warning::
  Some read-only properties in a xolotl object may not be copied over. 


Test coverage
^^^^^^^^^^^^

add is tested in: 




.. _benchmark:

benchmark
^^^^^^^^^
performs a quick benchmarking of a given ``xolotl`` model. ``benchmark`` first varies the simulation time step, and measures how quickly the model integrates. It then varies ``t_end``, and measures how fast it integrates at a fixed ``sim_dt``. Usage ::
  x.benchmark;


Test coverage
^^^^^^^^^^^^

benchmark is tested in: 




.. _checkCompartmentName:

checkCompartmentName
^^^^^^^^^^^^^^^^^^^^
is used internally by ``xolotl`` to verify that the compartment name you are using is valid and legal. This method is called every time you add a compartment to a ``xolotl`` object. 


Test coverage
^^^^^^^^^^^^

checkCompartmentName is tested in: 




.. _cleanup:

cleanup
^^^^^^^
Usage ::
  xolotl.cleanup
  x.cleanup
A static method that cleans up all transpiled ``C++`` and compiled binary files. Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 



Test coverage
^^^^^^^^^^^^

cleanup is tested in: 




.. _compile:

compile
^^^^^^^
compiles a executable binary form a transpiled ``C++`` file. These are stored in your ``xolotl`` directory. ``xolotl`` automatically compiles when it needs to. You can turn this functionality off by setting ::
  x.skip_hash = true;
In addition, creating a ``xolotl`` object through a function call does not automatically hash and compile. In this case, you should use ``x.md5hash``.
.. warning::
  If you turn hashing off, ``xolotl`` might not compile


Test coverage
^^^^^^^^^^^^

compile is tested in: 




.. _contributingCurrents:

contributingCurrents
^^^^^^^^^^^^^^^^^^^^
calculates the contributions of each current at every point in a voltage trace. This is used internally in ``xolotl.plot`` to color voltage traces. Syntax ::
  curr_index = contributingCurrents(V, I)
where V is a vector of voltages, I is the corresponding matrix of currents %


Test coverage
^^^^^^^^^^^^

contributingCurrents is tested in: 




.. _copy:

copy
^^^^
copies a xolotl object. ``copy`` creates an identical copy of a xolotl object that can be manipulated seperately. Both copies will use the same binary to integrate, unless you add a new component to one of them. Syntax ::
  x2 = copy(x);
.. warning::
  Some read-only properties in a xolotl object may not be copied over. 


Test coverage
^^^^^^^^^^^^

copy is tested in: 




.. _findNSpikes:

findNSpikes
^^^^^^^^^^^
static method of ``xolotl`` that computes the number of spikes in a voltage trace. Example use ::
  f = findNSpikes(V);
  f = findNSpikes(V, on_off_thresh)
``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0. 


Test coverage
^^^^^^^^^^^^

findNSpikes is tested in: 




.. _:



Test coverage
^^^^^^^^^^^^

 is tested in: 




