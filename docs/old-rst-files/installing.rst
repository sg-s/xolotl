.. highlight:: matlab

*************************************
Installing, Updating and Uninstalling
*************************************

There are multiple ways to install this toolbox, based on your level of expertise and the toolchain on your computer.

Installing: Download a MATLAB Toolbox
=====================================

``xolotl`` is available as a MATLAB toolbox. Click here_ to download it, and click on the downloaded file to install. 

.. _here: https://github.com/sg-s/xolotl/releases/download/latest/xolotl.mltbx

.. warning::

   If you wish to develop ``xolotl`` further, you should probably use git (see below).


Installing: Via Git
=====================

If you are comfortable with ``git``, you can clone all the code and dependencies yourself: ::

  git clone https://github.com/sg-s/srinivas.gs_mtools
  git clone https://github.com/sg-s/puppeteer
  git clone https://github.com/sg-s/xolotl
  git clone https://github.com/sg-s/cpplab

You will have to manually set your ``MATLAB`` paths. Make sure you add the main folder for ``puppeteer``, ``cpplab``, and ``xolotl``, and all subfolders of ``srinivas.gs_mtools/src``.



.. warning::

   If you want to develop ``xolotl`` further, make sure you're running a non-Windows OS, and that you make the `pre-commit` git hook executable: ``chmod a+x ./git/hooks/pre-commit``

Updating 
========

In most cases, ``xolotl`` can update itself to the latest version using ::

   xolotl.update()

If you installed using ``git``, ``xolotl`` will attempt to do a ``git pull`` and update itself. If you installed it as a MATLAB toolbox, ``xolotl`` will delete the old toolbox, download the new one, and install that. 


Uninstalling
============

If you installed ``xolotl`` as a MATLAB toolbox, you can easily uninstall it using :: 

   xolotl.uninstall()

Note that this doesn't do anything if you installed using ``git``, or if you manually downloaded the files and linked them. 
