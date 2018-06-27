%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

transpile
^^^^^^^^^

Generate a C++ file that constructs the model, integrates it, and moves parameters and data from MATLAB to C++ and back. Usage ::

   x.transpile;


.. warning::

``transpile`` assumes that your ``xolotl`` object has a valid hash. Empty hashes will throw an error. 


Example
-------

    % assuming a xolotl object is set up
    x.transpile;

    % now view the transpiled code
    x.viewCode;

	

See Also
--------

- xolotl.compile
- xolotl.viewCode


%}


function transpile(self)

h = self.hash;
out_file = ['mexBridge' h(1:6) '.cpp'];
self.transpileCore('mexTemplate.cpp',out_file)


