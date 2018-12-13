%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

# rebase

rebase is an internal method that configures some 
house-keeping settings. rebase is called every 
time a new xolotl object is created. rebase:

1. configures the `xolotl_folder` property
2. configures the `cpp_folder` property, which tells xolotl where its C++ files are located
3. configures the `OS_binary_ext`, which is the extention of compiled binaries and is platform dependent. 
4. calls the rebase method from the cpplab superclass.

Usage:

```
x.rebase()
```

### See also:

->cpplab.rebase()

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