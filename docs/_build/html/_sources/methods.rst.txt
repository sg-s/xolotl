.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _methods:

Methods
=======

This documentation will focus on using ``xolotl`` in ``MATLAB``, though it is
possible to compile and run the ``C++`` code on its own.

A ``xolotl`` object can be instantiated ::

  x = xolotl;

For consistency, this documentation will assume a ``xolotl`` object named ``x``.

.. hint::

  You can list all the methods of ``xolotl`` object ``x`` by ::

    x.methods

.. _children:

Children
^^^^^^^^

Returns the children of any ``cpplab`` object as a cell of character vectors.
Children are also ``cpplab`` objects. For example, if ``HH`` is the only compartment in ``x`` then ::

  x.Children

returns ::

  {'HH'}

This method is inherited from the superclass ``cpplab``.

.. _add:

add
^^^

Adds a compartment, conductance, or synapse to the ``xolotl`` object. To add
a compartment::

  x.add('compartmentName','compartment','PropertyName','PropertyValue', ...)

Valid compartment properties are

======================= ================
Name                    ``PropertyName``
Volume                  ``vol``
Surface area            ``A``
Extra. calcium steady.  ``Ca_out``
Buffer shell thickness  ``shell_thickness``
Intra. calcium steady.  ``Ca_in``
Ca++ curr. to conc.     ``phi``
Average Ca++ conc.      ``Ca_average``
Radius of compartment   ``radius``
Target Ca++ conc.       ``Ca_target``
Index in `xolotl` tree  ``tree_idx``
Index of neuron         ``neuron_idx``
Length of compartment   ``len``
Ca++ buff. time const.  ``tau_Ca``
Membrane capacitance    ``Cm``
Intra. calcium conc.    ``Ca``
Membrane potential      ``V``
======================= ================

To add a compartment named ``HH`` with a membrane capacitance ``Cm = 10`` ::

  x.add('HH', 'compartment', 'Cm', 10);

To access properties of that compartment, use the syntax ``x.HH``. To add a conductance to that compartment ::

  x.HH.add('ConductancePath', 'PropertyName', 'PropertyValue', ...);

``ConductancePath`` is the path to the ``.hpp`` file inside ``xolotl/c++/conductances/``. For example, the conductance path to add the slow calcium conductance from Prinz *et al.* 2003 is ``prinz/CaS`` since the conductance is specified in ``xolotl/c++/conductances/prinz/CaS.hpp``.

The following properties can be specified

======================= ================
Name                    ``PropertyName``
Maximal conductance     ``gbar``
Reversal potential      ``E``
Inactivation variable   ``h``
Activation variable     ``m``
======================= ================

For example ::

  x.HH.add('prinz/CaS', 'gbar', 500, 'E', -80);

The maximal conductance defaults to 0, the reversal potential defaults based on
the conductance. Activation variables (if any) default to zero. Inactivation variables
default to one. Synapses can be added this way, but it is better to use the ``connect`` method.

.. _checkCompartmentName:

checkCompartmentName
^^^^^^^^^^^^^^^^^^^^

Checks whether a given compartment name is on a list of illegal names. Illegal names for a new compartment include ``compartment``, ``synapses``, and any compartment name already specified in the ``xolotl`` object. ::

  BOOL = x.checkCompartmentName('CompartmentName');

.. _cleanup:

cleanup
^^^^^^^

When the ``C++`` code compiles, it produces a binary ``.mexa64`` file and a ``.cpp`` file in your ``xolotl`` directory. These files are hashed so that repeated simulation does not require recompilation. If these files are too numerous or broken, you can erase them all by calling either ::

  xolotl.cleanup
  x.cleanup


.. _compile:

compile
^^^^^^^

Hashes and compiles the files needed to run a simulation. These are stored in
your ``xolotl`` directory. ``xolotl`` automatically compiles when it needs to.
You can turn this functionally off by setting ::

  x.skip_hash = true;

In addition, creating a ``xolotl`` object through a function call does not
automatically hash and compile. In this case, you should use ``x.md5hash``.

.. warning::

  Always transpile before you compile! ::

    x.transpile;
    x.compile;

.. _connect:

connect
^^^^^^^

Connects two compartments with a synapse. The basic syntax is ::

  x.connect('PreSynaptic', 'PostSynaptic', 'Type', PropertyName', PropertyValue, ...)

The first two arguments are the presynaptic and postsynaptic compartment names.

In the case of two arguments, or three arguments where the third is a ``double``,
an axial or electrical synapse is created between the two compartments. An axial
synapse is created if either of the compartments is part of a spacially-discretized
multi-compartment structure (e.g. has a defined ``tree_idx``). Otherwise, the created
synapse is electrical. Axial and electrical synapses differ in how they are integrated
(see Dayan & Abbott 2001, Ch. 5-6). ::

  % create a synapse between AB and LP with gbar of NaN
  x.connect('AB', 'LP')
  % create a synapse between AB and LP with gbar of 10
  x.connect('AB', 'LP', 10)

The most common way to produce a synapse is to pass the synapse type and then any
properties. For example, to add a glutamatergic synapse between ``AB`` and ``LP``
with a maximal conductance of 100: ::

  x.connect('AB', 'LP', 'Glut', 'gbar', 100)


Synapses can also be connected by passing a ``cpplab`` object to the ``connect``
function ::

  % create a synapse using the cpplab object 'syn_cpplab'
  x.connect('AB', 'LP', syn_cpplab)

The following properties can be specified

======================= ================
Name                    ``PropertyName``
Maximal conductance     ``gbar``
Reversal potential      ``E``
Activation variable     ``s``
======================= ================

.. _copy:

copy
^^^^

Copies the entirety of a ``cpplab`` object into a new variable. ::

  % create a new xolotl object
  N = x.copy
  % create a new HH object
  N = x.HH.copy

.. _get:

get
^^^

Returns a vector of doubles of the values stored in the specified fields. Automatically calls the ``find`` function; the argument is a search query for the ``find`` function. ::

  % find all maximal conductances
  gbars = x.get('*gbar')
  % find all maximal conductances in HH compartment
  gbars = x.get('HH*gbar')

.. _find:

find
^^^^

Returns a cell array of character vectors for a search query. This function is a ``cpplab`` method, so what it does is specific to in which scope it is called.
For example, for a xolotl object ``x`` with a compartment ``HH`` with three conductances ``NaV, Kd, Leak`` ::

  x.find('*gbar')

returns ::

  {'HH.Kd.gbar'  }
  {'HH.Leak.gbar'}
  {'HH.NaV.gbar' }

whereas, a call within the ``HH`` scope ::

  x.HH.find('*gbar')

yields ::

  {'Kd.gbar'  }
  {'Leak.gbar'}
  {'NaV.gbar' }

The ``*`` symbol is the wildcard operator. It finds any string that matches that pattern (e.g. ``*gbar`` finds any string in the ``xolotl`` tree that ends with ``'gbar'``).
For example, ``x.find('HH*gbar')`` would only fetch paths to maximal conductances in the ``HH`` compartment.

.. hint::

  The ``set`` and ``get`` functions use the same syntax as the ``find`` function. They call it implicitly.

.. _getGatingFunctions:

getGatingFunctions
^^^^^^^^^^^^^^^^^^

Returns the gating functions of a conductance as function handles. The function has four outputs.
The activation steady state equation comes first, followed by the inactivation steady state (if any), then the activation time constant, and finally the inactivation time constant (if any).
The argument is a string that specifies where in the file directory the conductance is specified.
For example to find the ``NaV`` conductance from Liu *et al.* 1998, which is detailed in
``.../xolotl/c++/conductances/liu/NaV.hpp`` ::

  [m_inf, h_inf, tau_m, tau_h] = x.getGatingFunctions('liu/NaV')

.. hint::

  This is a static method of ``xolotl`` (i.e. ``xolotl.getGatingFunctions(...)`` is valid).

.. _integrate:

integrate
^^^^^^^^^

Integrates the ``xolotl`` object. Returns the membrane potential, intracellular calcium, controller states, intrinsic currents, and synaptic currents as time series.

The membrane potential is returned as matrix of ``time steps x compartments`` in the same order as the ``xolotl`` object's scalar representation (i.e. in the command window).
The intracellular calcium is returned as a matrix of ``time steps x 2*compartments``. The first *n* time series are the intracellular calcium concentrations for each compartment, and the second *n* time series are the instantaneous calcium reversal potentials.
Controller states are returned as a matrix of ``time steps x 2*controllers`` where the first column in each two-column pair is the controller state and the second is the state of the thing upon which the controller is operating.
For a controller onto a maximal conductance, for instance, the first column is the controller state and the second is the maximal conductance.
All other outputs are ``time series x XYZ`` where XYZ is the serialized list of those properties. For example, in a ``xolotl`` object with 2 compartments ``AB`` and ``BC``
with two conductances each: ``NaV`` and ``Kd``, the intrinsic currents would return in the form of a ``time steps x 4`` matrix where the columns would be ordered:
Kd current from AB, NaV current from AB, Kd current from BC, NaV current from BC. ::

  [V, Ca, cont_states, currents, syn_currents] = x.integrate

If ``integrate`` is passed one argument, it interprets this as a matrix of injected currents (in nA). For example ::

  % applies 0.2 nA to each compartment
  x.integrate(0.2)
  % applies 0.2 nA to first compartment of three
  x.integrate([0.2 0 0])
  % applies a time-varying current to first compartment
  % where time is a vector of length x.t_end / x.dt
  x.integrate([0.2*sin(time); zeros(time); zeros(time)])

If ``integrate`` is passed two arguments, the first is ignored, and the second is treated as a matrix of clamped voltage. ::

  % fixes the voltage of all compartments at -50 mV
  x.integrate([], -50])
  % clamps the second compartment in a two-compartment network
  x.integrate([], [NaN -50] )

.. _manipulate:

manipulate
^^^^^^^^^^

Opens the GUI to permit real-time visualization of changing network parameters.
The first argument specifies for which network properties to generate sliders (default is all of them). This takes the form of a call to the find_ function.
The second argument specifies which visualization function should be used (default is ``plot``). ::

  % minimum usage
  x.manipulate
  % specify sliders only for maximal conductances of HH compartment
  x.manipulate('HH*gbar')
  % specify the myPlot function for visualization
  x.manipulate('HH*gbar', @myPlot)

.. _plot:

plot
^^^^

.. _reset:

reset
^^^^^

.. _set:

set
^^^

.. _slice:

slice
^^^^^

.. _viewCode:

viewCode
^^^^^^^^
