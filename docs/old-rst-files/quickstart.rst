.. highlight:: matlab

**********
Quickstart
**********

This quickstart guide will get you started using ``xolotl`` right away. It will cover installation and basic usage. 

Installing
^^^^^^^^^^

The easiest way to get ``xolotl`` is as a MATLAB toolbox. Click here_ to download it, and click on the downloaded file to install. 

.. _here: https://github.com/sg-s/xolotl/releases/download/latest/xolotl.mltbx


Creating a Hodgkin-Huxley Model
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We will now create a ``xolotl`` object that describes a single-compartment neuron model with a fast sodium conductance, delayed rectifier potassium conductance, and passive leakage.
The compartment has a membrane capacitance ``'Cm'`` of 10 nF/mm^2 and a surface area of 0.01 mm^2.
These conductances come from Liu *et al.* 1998. ``'gbar'`` is the maximal conductance in uS/mm^2 and ``'E'`` is the reversal potential in mV. ::

  % create the xolotl object
  x = xolotl;

  % add a compartment
  x.add('compartment', 'HH', 'Cm', 10, 'A', 0.01);

  % add conductances
  x.HH.add('liu/NaV', 'gbar', 1000, 'E', 50);
  x.HH.add('liu/Kd', 'gbar', 300, 'E', -80);
  x.HH.add('Leak', 'gbar', 1, 'E', -40);

Simulate the Model
^^^^^^^^^^^^^^^^^^

We simulate the model using the GUI to manipulate the leak conductance. ::

  x.t_end = 1000; % ms
  x.manipulate('*Leak*')


You should get a GUI that pops up showing the voltage trace, with sliders that allow you to vary parameters for the Leak conductance. Play with the sliders and see what happens! 
