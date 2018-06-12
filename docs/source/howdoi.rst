.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _howdoi:
How do I...
===========


...create a empty xolotl object?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

  x = xolotl;


This is a prerequisite to doing anything else. (You can name your object whatever you want, but this documentation will assume that you've named it ``x``).


...create a new compartment?
^^^^^^^^^^^^^^^^^^^^^^^^^

You can first create a new compartment using ``cpplab`` ::

 AB = cpplab('compartment','vol',.01);


At this point you have created a free-floating object. You can inspect it just like you would any other ``MATLAB`` object: ::

  AB

  AB =

   compartment object with:

               hash : 5549561
                vol : 0.01
         neuron_idx : NaN
          Ca_target : NaN
              Ca_in : NaN
         Ca_average : NaN
                 Cm : NaN
                phi : NaN
             radius : NaN
                  V : NaN
                len : NaN
                 Ca : NaN
    shell_thickness : NaN
             tau_Ca : NaN
             Ca_out : NaN
                  A : NaN
           tree_idx : NaN

and then add it to the ``xolotl`` object tree: ::

  x.add('AB',AB);


A handy shortcut for this is: ::

  x.add('AB','compartment','vol',.01)


This shortcut syntax will be used for the rest of the documentation, but remember that you can also do things the "long" way.


...add a conductance to a compartment?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Assuming you have a compartment called ``AB`` in your ``xolotl`` object, ::

 x.AB.add('liu/NaV');


The string ``liu/NaV`` specifies the path within the ``C++`` folder that indicates where the header file for the thing that we want to add is.

Once again, you can inspect this object just like you would any other ``MATLAB`` object: ::

  x.AB.NaV

  ans =

   NaV object with:

         hash : 49007f5
            E : NaN
            m : NaN
         gbar : NaN
            h : NaN

As always, you can set these properties after-the-fact (e.g. ``x.gbar = 100``), or when the conductance is added
(e.g. ``x.add('liu/NaV', 'gbar', 100)``).


...find out what conductances are available?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Look in the folder yourself! All ``C++`` headerfiles are contained in the ``C++`` folder in the ``xolotl`` directory. If you are unsure where that is, type this in your ``MATLAB`` prompt: ::

  fileparts(fileparts(which('xolotl')))

...inspect the object I have created?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can inspect any object by outputting it in the command window. For example, to inspect the whole xolotl object ::

  >> xolotl object with
  ---------------------
  + HH
    > NaV (g=NaN, E=NaN)
  ---------------------

You can click on the linked (blue) text to inspect those properties, or reference them directly (e.g. ``x.AB.NaV``).

...connect two compartments using a synapse?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Connect two compartments with an electrical synapse ::

  x.connect('AB', 'PD')

Connect two compartments with an electrical synapse and specify properties ::

  x.connect('AB' 'PD', 'gbar', 100)

Connect two compartments with a glutamatergic synapse ::

  x.connect('AB', 'LP', 'prinz/Glut')

Connect two compartments with a glutmatergic synapse and specify properties ::

  x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)

...find out what synapse types are available?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Look in the folder yourself! All ``C++`` headerfiles are contained in the ``c++/synapses`` folder in the ``xolotl`` directory. If you are unsure where that is, type this in your ``MATLAB`` prompt: ::

  fileparts(fileparts(which('xolotl')))

...inject current into a compartment?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Add a scalar, vector, or matrix to ``x.I_ext``. This is interpreted as an injected current in nanoamperes.

Inject a constant current into all compartments ::

  x.I_ext = 0.2;

Inject a constant current into one of two compartments ::

  x.I_ext = [0.2 0];

Inject a variable current into one of two compartments ::

  nSteps = x.t_end / x.dt;
  I_ext  = zeros(nSteps, 2);
  I_ext(:,1) = 0.2 * rand(nSteps, 1);
  x.I_ext = I_ext;

...voltage clamp a compartment?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Add a matrix to ``x.V_clamp``. This is interpreted as an ``nSteps x nComps`` matrix of clamped voltage, where
``nSteps`` is the number of time-steps in the simulation, and ``nComps`` is the number of compartments.

Clamp the voltage and step it from -50 mV to 50 mV and back ::

  nSteps = x.t_end / x.dt;
  V_clamp = -50 * ones(nSteps, 1);
  V_clamp(ceil(nSteps/2), 1) = 50;
  V_clamp(ceil(nSteps*3/4), 1) = -50;
  x.V_clamp = V_clamp;

...specify simulation time step and other integration parameters?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Specify the time step by setting ``x.dt`` in milliseconds. Specify the simulation time by setting
``x.t_end`` in milliseconds. Injected current and voltage clamp are determined by setting ``x.I_ext`` and ``x.V_clamp``.

Set the simulation time to 5000 ms ::

  x.t_end = 5000

Set the time step to 0.1 ms ::

  x.dt = 0.1

...integrate the model and obtain outputs?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Integrate the model ::

    [V, Ca, cont_states, currents, syn_currents] = x.integrate

``V`` is the voltage trace as a matrix ``nSteps x nComps``. ``Ca`` is the intracellular calcium concentration trace.
``cont_states`` is the controller states and controlled parameters as time series. ``currents`` and ``syn_currents``
are the time traces of all the currents and synaptic currents, in the order that they are displayed in the serialized ``xolotl``
object (e.g. how ``x`` displays them in the command window).

...debug a model or simulation?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``xolotl`` has a debug mode that can be turned on using ::

  x.debug_mode = true;