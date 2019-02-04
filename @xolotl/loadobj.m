function b = loadobj(a)

a.xolotl_folder = fileparts(fileparts(which('xolotl')));
a.cpp_folder = [fileparts(fileparts(which('xolotl'))) filesep 'c++'];
b = a;