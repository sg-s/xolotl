.. highlight:: matlab

**********
Quickstart
**********

Welcome to ``xolotl``! This quickstart guide will get you started using ``xolotl`` right away.

Installing
^^^^^^^^^^

Get this repo from within ``MATLAB`` using my package manager ::

  % copy and paste this code in your MATLAB prompt
  urlwrite('http://srinivas.gs/install.m','install.m');
  install sg-s/srinivas.gs_mtools % you'll need this
  install sg-s/puppeteer % for manipulation
  install sg-s/cpplab
  install sg-s/xolotl

.. warning::

  Make sure that ``MATLAB`` is set up to compile_ ``C++``. See our compilers_ guide.

.. _compile: https://www.mathworks.com/help/matlab/ref/mex.html?s_tid=srchtitl
.. _compilers: compilers.html

Creating a Hodgkin-Huxley Model
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We will now create a ``xolotl`` object that describes a single-compartment neuron model with a fast sodium conductance, delayed rectifier potassium conductance, and passive leakage.
The compartment has a membrane capacitance ``'Cm'`` of 10 nF/mm^2 and a surface area of 0.01 mm^2.
These conductances come from Liu *et al.* 1998. ``'gbar'`` is the maximal conductance in uS/mm^2 and ``'E'`` is the reversal potential in mV. ::

  % create the xolotl object
  x = xolotl;
  % add a compartment
  x.add('HH', 'compartment', 'Cm', 10, 'A', 0.01);
  % add conductances
  x.HH.add('liu/NaV', 'gbar', 1000, 'E', 50);
  x.HH.add('liu/Kd', 'gbar', 300, 'E', -80);
  x.HH.add('Leak', 'gbar', 1, 'E', -40);

Simulate the Model
^^^^^^^^^^^^^^^^^^

We simulate the model using the GUI to manipulate the maximal conductances. ::

  x.t_end = 1000; % ms
  x.manipulate('*gbar')
