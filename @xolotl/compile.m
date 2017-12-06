%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: compiles a C++ file using mex
% 

function [] = compile(self)
	shc = self.skip_hash_check;
	self.skip_hash_check = false;
	h = self.hash;
	self.skip_hash_check = shc;

	xolotl_folder = fileparts(fileparts(which(mfilename)));
	cpp_folder = joinPath(xolotl_folder,'c++');

	mexBridge_name = [joinPath(xolotl_folder,'mexBridge') h(1:6) '.cpp'];
	assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')
	mex('-silent',mexBridge_name,'-outdir',xolotl_folder)
	% update linked_binary

	self.linked_binary = [pathEnd(mexBridge_name) '.' self.OS_binary_ext];

	% also compile the NOCL version
	mexBridge_name = [joinPath(xolotl_folder,'mexBridge') h(1:6) 'NOCL.cpp'];
	assert(exist(mexBridge_name,'file')==2,'NOCL C++ file to compile does not exist. Use "transpile" before compiling')
	mex('-silent',mexBridge_name,'-outdir',xolotl_folder)

end