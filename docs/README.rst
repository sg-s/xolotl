
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
architecture of the underlying ``C++`` code. For example, here, we set up a
compartment with some channels in it: ::

  x = xolotl;
  x.add('compartment', 'HH', 'Cm', 10, 'A', 0.01);
  x.HH.add('liu/NaV', 'gbar', 1000);
  x.HH.add('liu/Kd', 'gbar', 300);
  x.HH.add('Leak', 'gbar', 0.1);
  x.I_ext = 0.1;

That's it. To integrate it and see the time evolution of the voltage ::

  V = x.integrate;

.. figure:: https://user-images.githubusercontent.com/6005346/30713658-ff96faf4-9edd-11e7-9db1-a2ca4f2f0567.png

``xolotl`` handles all of the compilation/linking/etc. for you.

2. It's Fast
^^^^^^^^^^^^
Because ``xolotl`` is written in ``C++``, it's quite fast. Here are some benchmarks
for a single-compartment Hodgkin-Huxley model with 7 conductances.
Note that you can get speeds up to 300x realtime!

.. figure:: https://user-images.githubusercontent.com/6005346/38319588-223ab634-3800-11e8-954c-845c0692d4a0.png

3. Manipulate Models in Real-time
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif

4. Optimize Models Really Quickly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

(coming soon)
