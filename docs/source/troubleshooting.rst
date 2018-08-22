.. highlight:: matlab


Troubleshooting
***************


I ran the quickstart, but I don't see anything
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Are you using a tiny screen? Some UI elements may go out of the frame on very small screens. To fix this, acquire the handle to the figure and change the position property. For example ::

  x.manipulate;
  manip = gcf;
  manip.Position = [100 100 34 56];

I get an error saying I don't have a compiler
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You need a C/C++ compiler. You need to follow MATLAB's instructions_ on how to get one, how to install one, and how to configure one. It may be helpful to also see our advice on compilers_.

.. _instructions: https://www.mathworks.com/support/compilers.html
.. _compilers: compilers.rst
