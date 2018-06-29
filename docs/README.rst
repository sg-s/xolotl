
**********************************************
XOLOTL: A Fast and Flexible Neuronal Simulator
**********************************************

.. highlight:: matlab

.. image:: https://readthedocs.org/projects/sphinx-rtd-theme/badge/?version=latest
  :target: http://sphinx-rtd-theme.readthedocs.io/en/latest/?badge=latest
  :alt: Documentation Status

.. image:: https://img.shields.io/github/last-commit/sg-s/xolotl.svg

.. image:: https://user-images.githubusercontent.com/6005346/41205222-30b6f3d4-6cbd-11e8-983b-9125585d629a.png

``xolotl`` is a fast single-compartment and multi-compartment simulator written in
``C++`` with a ``MATLAB`` interface. Designed with a focus on ease-of-use, flexibility and speed,
``xolotl`` simulates conductance-based neuron models and networks.

Why Use ``xolotl``?
===================

1. It's Easy to Use
^^^^^^^^^^^^^^^^^^^

You can set up complex models of neurons and networks very efficently,
using an intuitive language that is tightly coupled to the object-based
architecture of the underlying ``C++`` code.

.. _model:

For example, here, we set up a
compartment with some channels in it: ::

  x = xolotl;
  x.add('compartment', 'HH', 'Cm', 10, 'A', 0.01);
  x.HH.add('liu/NaV', 'gbar', 1000);
  x.HH.add('liu/Kd', 'gbar', 300);
  x.HH.add('Leak', 'gbar', 0.1);
  x.I_ext = 0.1;

That's it. To integrate it and see the time evolution of the voltage ::

  V = x.integrate;

.. figure:: https://user-images.githubusercontent.com/30243182/42067394-0801376e-7b14-11e8-8eb3-b20b64ffaf9f.png

``xolotl`` handles all of the compilation/linking/etc. for you.

2. It's Fast
^^^^^^^^^^^^
Because ``xolotl`` is written in ``C++``, it's quite fast. Here are some benchmarks
for a single-compartment Hodgkin-Huxley model_ with sodium, potassium, and passive leak conductances. ::

  x.benchmark;

.. figure:: https://user-images.githubusercontent.com/30243182/42105769-9c1c407a-7b9f-11e8-86fb-8fc56eac9ff6.png

3. Manipulate Models in Real-time
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Networks in ``xolotl`` can be manipulated in real-time using the graphical interface. Any parameter
in ``xolotl`` can be accessed by a slider. Simulations are displayed by a chosen visualization function.
Here, we manipulate the synaptic maximal conductances of a `three neuron model`__ and display using
the ``xolotl`` ``plot`` function.

.. _Prinz: https://www.ncbi.nlm.nih.gov/pubmed/15558066
__ Prinz_

.. figure:: https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif

4. Optimize Models Really Quickly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``xolotl`` uses ``MATLAB``'s Global Optimization Toolbox to optimize model parameters
to satisfy arbitrary cost functions. The ``procrustes`` toolbox for ``xolotl`` optimizes
chosen parameters in parallel to fit a cost function using pattern search, particle swarm, or
a genenetic algorithm.
