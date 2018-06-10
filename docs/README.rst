
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
``C++`` with ``MATLAB`` wrappers. Written with a focus on flexibility and speed,
it can simulate single-compartment conductance-based neuron models, networks of these,
and detailed multi-compartment models. Because it's written in ``C++`` it's really fast.

Why Use ``xolotl``?
===================

1. It's Fast
^^^^^^^^^^^^
Because ``xolotl`` is written in ``C++``, it's quite fast. Here are some benchmarks
for a single-compartment Hodgkin-Huxley model with 7 conductances.
Note that you can get speeds up to 300x realtime!

.. figure:: https://user-images.githubusercontent.com/6005346/38319588-223ab634-3800-11e8-954c-845c0692d4a0.png

2. It's Easy to Use
^^^^^^^^^^^^^^^^^^^

You can set up complex models of neurons and networks very efficently,
using an intuitive language that is tightly coupled to the object-based
architecture of the underlying ``C++`` code. For example, here, we set up a
compartment with some channels in it: ::

  x = xolotl;
  x.add('AB','compartment','Cm',10,'A',0.0628,'vol',1,'phi',19,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',200);
  x.AB.add('prinz-approx/NaV','gbar',1000,'E',50);
  x.AB.add('prinz-approx/Kd','gbar',1000,'E',-80);

That's it. To integrate it and see the time evolution of the voltage ::

  V = x.integrate;

.. figure:: https://user-images.githubusercontent.com/6005346/30713658-ff96faf4-9edd-11e7-9db1-a2ca4f2f0567.png

``xolotl`` handles all of the compilation/linking/etc. for you.

3. Manipulate Models in Real-time
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif

4. Optimize Models Really Quickly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``xolotl`` comes with a powerful optimization toolbox called procrustes_.

.. _procrustes: https://github.com/sg-s/procrustes

NB: This repository is currently hidden (embargoed).

How to Get ``xolotl``
=====================

Get this repo from within ``MATLAB`` using my package manager ::

  % copy and paste this code in your MATLAB prompt
  urlwrite('http://srinivas.gs/install.m','install.m');
  install sg-s/srinivas.gs_mtools % you'll need this
  install sg-s/puppeteer % for manipulation
  install sg-s/cpplab
  install sg-s/xolotl

Via Git or Download
^^^^^^^^^^^^^^^^^^^

Use git if you plan to develop this further: ::

  git clone https://github.com/sg-s/srinivas.gs_mtools
  git clone https://github.com/sg-s/puppeteer
  git clone https://github.com/sg-s/xolotl
  git clone https://github.com/sg-s/cpplab

Don't forget to set your ``MATLAB`` paths if you used git! You need the main folder
for ``puppeteer``, ``cpplab``, and ``xolotl``, and all subfolders of ``srinivas.gs_mtools/src``.


.. warning::
  There are no known issues on macOS (10.12 and 10.13). For Linux distributions,
  you might need a compiler_ compatible with the ``mex`` ``MATLAB`` executable.

.. _compiler: compilers.html
