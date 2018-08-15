.. highlight:: matlab

************
Installing
************

There are multiple ways to install this toolbox, based on your level of expertise and the toolchain on your computer. 

Using a package manager from MATLAB
===================================

This is the simplest way of installing ``xolotl`` for most users. Simply copy and paste this code snippet into your ``MATLAB`` prompt and hit enter::

  % copy and paste this code in your MATLAB prompt
  urlwrite('https://git.io/f4SdA','install_xolotl.m');
  install_xolotl;

This script downloads ``xolotl`` and all dependencies, unzips them, puts them somewhere sensible, and adds those locations to your path. It also compiles some dependencies. ``xolotl`` should be ready to use. 

.. warning:: 

   If you wish to develop ``xolotl`` further, you should probably use git (see below). 


Via Git
=======

If you are comfortable with ``git``, you can clone all the code and dependencies yourself: ::

  git clone https://github.com/sg-s/srinivas.gs_mtools
  git clone https://github.com/sg-s/puppeteer
  git clone https://github.com/sg-s/xolotl
  git clone https://github.com/sg-s/cpplab

You will have to manually set your ``MATLAB`` paths. Make sure you add the main folder for ``puppeteer``, ``cpplab``, and ``xolotl``, and all subfolders of ``srinivas.gs_mtools/src``.



.. warning:: 

   If you want to develop ``xolotl`` further, make sure you're running a sensible (i.e., non-Windows) OS, and that you make the `pre-commit` git hook executable: ``chmod a+x ./git/hooks/pre-commit``


Manual Installation
===================

You should only do this if 

- you don't know how to use ``git``
- using the package manager (see above) didn't work

as this is the most cumbersome method of installing things, and should be avoided. 

For each of the following repos:

- https://github.com/sg-s/srinivas.gs_mtools
- https://github.com/sg-s/puppeteer
- https://github.com/sg-s/xolotl
- https://github.com/sg-s/cpplab

you will have to

1. click on the green "clone or download" button, choose "Download ZIP"
2. Unzip the download
3. Rename the folder to something sensible
4. Move that folder into some place sensible
5. Add that folder (and some subfolders) to your MATLAB path
6. Save your MATLAB path 

Make sure you add the main folder for ``puppeteer``, ``cpplab``, and ``xolotl``, and all subfolders of ``srinivas.gs_mtools/src``.