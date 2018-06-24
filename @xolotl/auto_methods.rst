.. _benchmark:

 benchmark
 =========

 performs a quick benchmarking of a given ``xolotl`` model. ``benchmark`` first varies the simulation time step, and measures how quickly the model integrates. It then varies ``t_end``, and measures how fast it integrates at a fixed ``sim_dt``. Usage ::

   x.benchmark;



Test coverage
-------------

benchmark is tested in: 




.. _checkCompartmentName:

 checkCompartmentName
 ====================

 is used internally by ``xolotl`` to verify that the compartment name you are using is valid and legal. This method is called every time you add a compartment to a ``xolotl`` object. 



Test coverage
-------------

checkCompartmentName is tested in: 




.. _cleanup:

 cleanup
 =======

 Usage ::

   xolotl.cleanup
   x.cleanup

 A static method that cleans up all transpiled ``C++`` and compiled binary files. Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 
 


Test coverage
-------------

cleanup is tested in: 


run_all_tests.m



.. _compile:

 compile
 =======

 compiles a executable binary form a transpiled ``C++`` file. These are stored in your ``xolotl`` directory. ``xolotl`` automatically compiles when it needs to. You can turn this functionality off by setting ::

   x.skip_hash = true;

 In addition, creating a ``xolotl`` object through a function call does not automatically hash and compile. In this case, you should use ``x.md5hash``.

 .. warning::

   If you turn hashing off, ``xolotl`` might not compile


Test coverage
-------------

compile is tested in: 




.. _connect:

 connect
 =======

 Connects two compartments with a synapse. The basic syntax is ::

   x.connect('Comp1', 'Comp2', 'SynapseType', ...)

 The first two arguments are the presynaptic and postsynaptic compartment names. For example ::

   % connects two different neurons with an electrical synapse
   x.connect('AB', 'LP')

 Axial synapses are a special type of electrical synapse that are created between spatially-discrete compartments in a morphological structure.
 Electrical and axial synapses differ in how they are integrated (see Dayan & Abbott 2001, Ch. 5-6).
 ``connect`` defaults to an axial synapse when the type of synapse is not specified and either compartment has a defined ``tree_idx`` (which identifies the compartment as a part of a multi-compartment neuron model)
 Otherwise, the created synapse is electrical. ::

   % create an (electrical or axial) synapse between AB and LP with gbar of NaN
   x.connect('AB', 'LP')
   % create an (electrical or axial) synapse between AB and LP with gbar of 10
   x.connect('AB', 'LP', 10)
 The most common way to produce a synapse is to pass the synapse type and then any properties. This is used to create chemical synapses. For example, to add a glutamatergic synapse (from Prinz *et al.* 2004) between ``AB`` and ``LP`` with a maximal conductance of 100: ::
   x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)
 Synapses can also be connected by passing a ``cpplab`` object to the ``connect`` method ::
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
-------------

connect is tested in: 


test_stg.m



.. _contributingCurrents:

 contributingCurrents
 ====================

 calculates the contributions of each current at every point in a voltage trace. This is used internally in ``xolotl.plot`` to color voltage traces. Syntax ::

   curr_index = contributingCurrents(V, I)

 where V is a vector of voltages, I is the corresponding matrix of currents %


Test coverage
-------------

contributingCurrents is tested in: 




.. _findNSpikes:

 findNSpikes
 ===========

 static method of ``xolotl`` that computes the number of spikes in a voltage trace. Example use ::

   f = findNSpikes(V);
   f = findNSpikes(V, on_off_thresh)

 ``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0. 



Test coverage
-------------

findNSpikes is tested in: 




