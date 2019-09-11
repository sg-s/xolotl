
% 
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
% 
% ### loadobj
% 
% **Description**
% 
% A static method that overloads the built-in loadobj method. This sets 
% local parameters, and is useful when xolotl objects are saved to
% disk and loaded. 
% 
% !!! warning 
% XXXXDo not use this method. This method exists so that MATLAB knows how to load xolotl objects correctly
% 
% 

function b = loadobj(a)

a.xolotl_folder = fileparts(fileparts(which('xolotl')));
a.cpp_folder = [fileparts(fileparts(which('xolotl'))) filesep 'c++'];

% update linked_binary extension 
[~,binary_name]=fileparts(a.linked_binary);
a.linked_binary = [binary_name '.' mexext];

b = a;