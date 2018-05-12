.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _installing:
Installing
==========

Using a package manager from MATLAB
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. set up matlab code highlighting
.. highlight:: matlab

Get this repo from within ``MATLAB`` using my package manager ::

  % copy and paste this code in your MATLAB prompt
  urlwrite('http://srinivas.gs/install.m','install.m');
  install sg-s/srinivas.gs_mtools % you'll need this
  install sg-s/puppeteer % for manipulation
  install sg-s/cpplab
  install sg-s/xolotl

Using ``git``
^^^^^^^^^^^^^

Use git if you plan to develop this further: ::

  git clone https://github.com/sg-s/srinivas.gs_mtools
  git clone https://github.com/sg-s/puppeteer
  git clone https://github.com/sg-s/xolotl
  git clone https://github.com/sg-s/cpplab



Don't forget to set your ``MATLAB`` paths if you used git!


Final Steps
^^^^^^^^^^^

Finally, make sure you `configure MATLAB so that it is set up to delete files permanently`__.

.. _MatlabDelete: https://www.mathworks.com/help/matlab/ref/delete.html

__ MatlabDelete_

Otherwise you will end up with a very large number of temporary files in your trash!
