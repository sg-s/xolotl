.. highlight:: matlab

************
Installing
************

There are multiple ways to install this toolbox, based on your level of expertise and the toolchain on your computer.


Download a MATLAB Toolbox
=========================

``xolotl`` is available as a MATLAB toolbox. Click here_ to download it, and click on the downloaded file to install. 

.. _here: https://drive.google.com/uc?id=1LRlTNWvtfV4BKYcuLjEdTeh-Cjr6oWKJ&export=download

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

   If you want to develop ``xolotl`` further, make sure you're running a non-Windows OS, and that you make the `pre-commit` git hook executable: ``chmod a+x ./git/hooks/pre-commit``

