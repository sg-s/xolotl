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

.. _copy:

copy
^^^^

.. _get:

get
^^^

.. _find:

find
^^^^

.. _getGatingFunctions:

getGatingFunctions
^^^^^^^^^^^^^^^^^^

.. _integrate:

integrate
^^^^^^^^^

.. _manipulate:

manipulate
^^^^^^^^^^

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
