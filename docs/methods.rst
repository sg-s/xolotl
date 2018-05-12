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

    ``x.methods``

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

Synapses can be added this way, but it is better to use the ``connect`` method.

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

Hashes and compiles the files needed to run a simulation. These are stored in your ``xolotl`` directory. ``xolotl`` automatically compiles when it needs to. You can turn this functionally off by setting ::

  x.skip_hash = true;

In addition, creating a ``xolotl`` object through a function call does not automatically hash and compile. In this case, you should use ``x.sha1hash``.

.. warning::

  Always transpile before you compile! ::

    x.transpile;

    x.compile;

.. _connect:

connect
^^^^^^^

Connects two compartments with a synapse. This defaults to an electrical synapse with axial conductance of ``NaN``. ::



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
