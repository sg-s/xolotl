.. highlight:: matlab

************
Installing
************

There are multiple ways to install this toolbox, based on your level of expertise and the toolchain on your computer. 

Using a package manager from MATLAB
===================================

This is the recommended way of installing ``xolotl`` for most users. Simply copy and paste this code snippet into your ``MATLAB`` prompt and hit enter::

  % copy and paste this code in your MATLAB prompt
  urlwrite('https://raw.githubusercontent.com/sg-s/xolotl/master/install_xolotl.m','install_xolotl.m');
  install_xolotl;

This script downloads ``xolotl`` and all dependencies, unzips them, puts them somewhere sensible, and adds those locations to your path. It also compiles some dependencies. ``xolotl`` should be ready to use. 

Via Git or Download
===================

If you are comfortable with ``git``, you can clone all the code and dependencies yourself: ::

  git clone https://github.com/sg-s/srinivas.gs_mtools
  git clone https://github.com/sg-s/puppeteer
  git clone https://github.com/sg-s/xolotl
  git clone https://github.com/sg-s/cpplab

You will have to manually set your ``MATLAB`` paths. Make sure you add the main folder for ``puppeteer``, ``cpplab``, and ``xolotl``, and all subfolders of ``srinivas.gs_mtools/src``.


