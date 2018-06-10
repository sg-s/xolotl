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

  x.add('PD','compartment','vol',.01)


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
  



...find out what conductances are available?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Look in the folder yourself! All ``C++`` headerfiles are contained in the ``C++`` folder in the ``xolotl`` directory. If you are unsure where that is, type this in your ``MATLAB`` prompt: ::

  fileparts(fileparts(which('xolotl')))

...inspect the object I have created?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

...connect two compartments using a synapse?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


...find out what synapse types are available? 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


...inject current into a compartment?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

...voltage clamp a compartment? 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


...specify simulation time step and other integration parameters?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

...integrate the model and obtain outputs? 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




