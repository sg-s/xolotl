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

In the rest of this documentation we will assume a ``xolotl`` object named ``x``.

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

======================= ==================
**Name**                **Property Name**
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
======================= ==================

To add a compartment named ``HH`` with a membrane capacitance ``Cm = 10`` ::

  x.add('HH', 'compartment', 'Cm', 10);

To access properties of that compartment, use the syntax ``x.HH``. To add a conductance to that compartment ::

  x.HH.add('ConductancePath', 'PropertyName', 'PropertyValue', ...);

``ConductancePath`` is the path to the ``.hpp`` file inside ``xolotl/c++/conductances/``. For example, the conductance path to add the slow calcium conductance from Prinz *et al.* 2003 is ``prinz/CaS`` since the conductance is specified in ``xolotl/c++/conductances/prinz/CaS.hpp``.
You do not have to specify the entire path. Any fragment of the path will be matched.

The following properties can be specified

======================= ==================
**Name**                **Property Name**
Maximal conductance     ``gbar``
Reversal potential      ``E``
Inactivation variable   ``h``
Activation variable     ``m``
======================= ==================

For example ::

  x.HH.add('prinz/CaS', 'gbar', 500, 'E', -80);

The maximal conductance defaults to 0, the reversal potential defaults based on
the conductance. Activation variables (if any) default to zero. Inactivation variables
default to one. Synapses can be added this way, but it is better to use the connect_ method.

.. note::

  ``cpplab`` allows you to add any ``cpplab`` object to anything else, but ``xolotl`` assumes that you're doing sane things. If you add a compartment to a conductance, or something silly, you will get an error.

.. _cleanup:

cleanup
^^^^^^^

When the ``C++`` code compiles, it produces a binary and a ``.cpp`` file in your ``xolotl`` directory. These files are hashed so that repeated simulation does not require recompilation. If these files are too numerous or broken, you can erase them all by calling either ::

  xolotl.cleanup
  x.cleanup


.. _compile:

compile
^^^^^^^

Hashes and compiles the files needed to run a simulation. These are stored in
your ``xolotl`` directory. ``xolotl`` automatically compiles when it needs to.
You can turn this functionality off by setting ::

  x.skip_hash = true;

In addition, creating a ``xolotl`` object through a function call does not automatically hash and compile. In this case, you should use ``x.md5hash``.

.. warning::

  If you turn hashing off, ``xolotl`` might not compile

.. _connect:

connect
^^^^^^^

Connects two compartments with a synapse. The basic syntax is ::

  x.connect('PreSynaptic', 'PostSynaptic', 'Type', PropertyName', PropertyValue, ...)

The first two arguments are the presynaptic and postsynaptic compartment names. ::

  % connects two compartments with an electrical or axial synapse
  x.connect('AB', 'LP')

Axial synapses are a special type of electrical synapse that are created between spatially-discrete compartments in a morphological structure.
Electrical and axial synapses differ in how they are integrated (see Dayan & Abbott 2001, Ch. 5-6).
``connect`` defaults to an axial synapse when the type of synapse is not specified and either compartment has a defined ``tree_idx``.
Otherwise, the created synapse is electrical. ::

  % create an (electrical or axial) synapse between AB and LP with gbar of NaN
  x.connect('AB', 'LP')
  % create an (electrical or axial) synapse between AB and LP with gbar of 10
  x.connect('AB', 'LP', 10)

The most common way to produce a synapse is to pass the synapse type and then any
properties. This is used to create chemical synapses.
For example, to add a glutamatergic synapse (from Prinz *et al.* 2004) between ``AB`` and ``LP``
with a maximal conductance of 100: ::

  x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)


Synapses can also be connected by passing a ``cpplab`` object to the ``connect``
function ::

  % create a synapse using the cpplab object 'syn_cpplab'
  x.connect('AB', 'LP', syn_cpplab)

The following properties can be specified

======================= ================
Name                    PropertyName
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
::

  values = x.get('findString')

Returns a vector of doubles of the values stored in the specified fields. Automatically calls the ``find`` function; the argument is a search query for the ``find`` function. ::

  % find all maximal conductances
  gbars = x.get('*gbar')
  % find all maximal conductances in HH compartment
  gbars = x.HH.get('*gbar')

Produces a vector which contains the values of specified properties. ``get`` implicitly calls ``find`` and uses it to identify and order the elements of the vector. For this reason, ``get`` uses wildcard string comprehension. For example, ``'HH*gbar'`` means anything that begins with ``'HH'`` and ends with ``'gbar'``.
Find all the maximal conductances of the ``HH`` compartment ::

  gbars = x.get('HH*gbar')

.. _find:

find
^^^^

::

  x.find('findString')

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

::

  [act ict tau_act tau_ict] = xolotl.getGatingFunctions('conductancePath')

Returns the gating functions of a conductance as function handles. The function has four outputs.
The activation steady state equation comes first, followed by the inactivation steady state (if any), then the activation time constant, and finally the inactivation time constant (if any).
The argument is a string that specifies where in the file directory the conductance is specified.
For example to find the ``NaV`` conductance from Liu *et al.* 1998, which is detailed in
``.../xolotl/c++/conductances/liu/NaV.hpp`` ::

  [m_inf, h_inf, tau_m, tau_h] = x.getGatingFunctions('liu/NaV')

These functions are also contained in the ``xolotl`` tree, e.g. ::

  x.HH.NaV.cpp_child_functions

.. hint::

  This is a static method of ``xolotl`` (i.e. ``xolotl.getGatingFunctions(...)`` is valid).

.. _integrate:

integrate
^^^^^^^^^

::

  [V, Ca, cont_states, currents, syn_currents] = x.integrate

Integrates the ``xolotl`` object. Returns the membrane potential, intracellular calcium, controller states, intrinsic currents, and synaptic currents as time series.

The membrane potential is returned as matrix of ``time steps x compartments`` in the same order as the ``xolotl`` object's scalar representation (i.e. in the command window).
The intracellular calcium is returned as a matrix of ``time steps x 2*compartments``. The first *n* time series are the intracellular calcium concentrations for each compartment, and the second *n* time series are the instantaneous calcium reversal potentials.
Controller states are returned as a matrix of ``time steps x 2*controllers`` where the first column in each two-column pair is the controller state and the second is the state of the thing upon which the controller is operating.
For a controller onto a maximal conductance, for instance, the first column is the controller state and the second is the maximal conductance.
All other outputs are ``time series x XYZ`` where XYZ is the serialized list of those properties. For example, in a ``xolotl`` object with 2 compartments ``AB`` and ``BC``
with two conductances each: ``NaV`` and ``Kd``, the intrinsic currents would return in the form of a ``time steps x 4`` matrix where the columns would be ordered:
Kd current from AB, NaV current from AB, Kd current from BC, NaV current from BC.

If the ``xolotl`` properties ``I_ext`` and ``V_clamp`` are empty (i.e. ``x.I_ext = []``), the integration proceeds without injected current or clamped voltage.
Injected current and voltage clamp cannot co-occur.  Setting one of these properties negates the other. If ``I_ext`` is a scalar,
current is injected into each compartment at every time step. ::

  % add 0.1 nA to each compartment at each time-step
  x.I_ext = 0.1;

If ``I_ext`` is non-scalar, it can take the form of a vector of size ``nComps x 1``, or a matrix of size ``nSteps x nComps``, where
``nSteps`` is the number of time-steps in the simulation (i.e. ``x_t_end/x.dt``) and ``nComps`` is the number of compartments.

The first form injects a constant current into each compartment, as specified elementwise by the vector. ::

  % apply 0.1 nA to first compartment, 0 to second
  x.I_ext = [0.1 0];

If ``I_ext`` is a matrix of size ``nSteps x nComps``, current is injected elementwise during simulation,
so that the injected current at each time step for each compartment is specfied
by an element in the matrix. ::

  % add a current step to the second half of the simulation
  Ie = zeros(nSteps, nComps); Ie(round(nSteps/2),1) = 0.1;
  x.I_ext = Ie;

Clamped voltage is always added to the ``xolotl`` structure in the matrix form. ::

  % voltage clamp the first compartment at -50 then 50
  Vc = zeros(nSteps, nComps); Vc(1:5000,1) = -50; Vc(5001:10000,1) = 50;
  x.V_clamp = Vc;

.. note::

  If you add new compartments to the network, it will reset the ``I_ext`` and ``V_clamp``
  properties of ``xolotl``.


.. _manipulate:

manipulate
^^^^^^^^^^

::

  x.manipulate('findString')

Opens the GUI to permit real-time visualization of changing network parameters.
An argument specifies for which network properties to generate sliders (default is all of them). ::

  % minimum usage
  x.manipulate
  % specify sliders only for maximal conductances of HH compartment
  x.manipulate('HH*gbar')

``manipulate`` displays the results of the ``plot`` function by default.
If the ``manipulate_plot_func`` property of ``xolotl`` is specified (e.g. ``x.manipulate_plot_func{1} = @myFunc``),
it will display custom functions. ``manipulate_plot_func`` is a cell of function handles to all visualzation functions
that are wrapped by ``manipulate``. ::

  % wrap two visualization functions with manipulate
  x.manipulate_plot_func{1} = @x.plot;
  x.manipulate_plot_func{2} = @myFIcurve;
  x.manipulate

.. note::

  Multiple visualzation functions with ``manipulate`` is current in-development.

.. _plot:

plot
^^^^

Plots voltage and intracellular calcium traces for each compartment.
The voltage traces are colored based on the dominant current. A current is dominant when
it is the outward current with the greatest magnitude and dV/dt is negative;
or it is the inward current with the greatest magnitude and dV/dt is positive. ::

  x.plot

.. _replicate:

replicate
^^^^^^^^^

::

  x.replicate('cpplabObject', N)

Replicates a ``cpplab`` object in the tree ``N`` times with all children included.
This is useful for generating many identical compartments. To generate 100 ``HH`` compartments,
where ``HH`` is already specified ::

  x.replicate('HH', 100)

.. _reset:

reset
^^^^^

Resets all state variables to their initial condition. State variables are the membrane potential and intracellular calcium concentration of each compartment, and gating variables for each conductance.
When the ``xolotl`` object is flagged ``x.closed_loop = false``, this is done automatically before integrating. ::

  x.reset

.. _set:

set
^^^

::

  x.set('findString', values)

Sets network parameters to specified values.
The first argument is a find_ string that indicates which parameters to set.
The second argument is a vector that holds the values for setting the parameters.
This function is useful for setting many parameters at once. For example, to set the maximal conductances of all compartments to a the values of a vector ``gbars`` ::

  x.set('*gbar', gbars)

.. _show:

show
^^^^

::

  xolotl.show('conductancePath')

Plots the activation and inactivation steady-states and time constants for a conductance.
The argument is a string that specifies where in the file directory the conductance is specified.
For example to show the ``NaV`` conductance from Liu *et al.* 1998, which is detailed in
``.../xolotl/c++/conductances/liu/NaV.hpp`` ::

  x.show('liu/NaV')

.. hint::

  This is a static method of ``xolotl``.

.. _slice:

slice
^^^^^

::

  x.slice(compartmentName, nSlices, axialResistivity)

Splits up a single compartment into many compartments connected by axial `synapses.`
This is only used in multicompartment models to add morphological complexity.

The first argument specifies which compartment to slice, as a character vector.
The second argument specifies the number of total slices. To make 100 compartments
connected by axial `synapses,` specify 100.
The third argument sets the axial resistivity, which must be a real positive number. It defaults to NaN.

.. hint::

  This function assumes cylindrical geometry (i.e. the ``radius`` and ``len`` properties of the compartment must be defined).

.. _viewCode:

viewCode
^^^^^^^^

::

  x.viewCode

Displays the ``C++ mexBridge`` code in your default editor.
