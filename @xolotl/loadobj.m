
%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### loadobj

**Description**

A static method that overloads the built-in loadobj method. This sets 
local parameters, and is useful when xolotl objects are saved to
disk and loaded. 

!!! warning 
    Do not use this method.

%}

function b = loadobj(a)

a.xolotl_folder = fileparts(fileparts(which('xolotl')));
a.cpp_folder = [fileparts(fileparts(which('xolotl'))) filesep 'c++'];
b = a;