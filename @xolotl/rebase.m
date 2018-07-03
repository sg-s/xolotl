%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

rebase
^^^^^^

Configures some internal house-keeping settings. This is called every time a new object is created. You probably don't ever have to use this, unless you copy ``xolotl`` objects across computers with different file systems or operating systems. Usage ::

   x.rebase()

%}


function [] = rebase(self)

	self.xolotl_folder = fileparts(fileparts(which('xolotl')));
	self.cpp_folder = joinPath(self.xolotl_folder,'c++');

	if ismac
		self.OS_binary_ext = 'mexmaci64';
	elseif ispc 
		self.OS_binary_ext = 'mexw64';
	else
		self.OS_binary_ext = 'mexa64';
	end

	% update all C++ class paths 
	rebase@cpplab(self);


end