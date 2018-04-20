%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: compiles a C++ file using mex
% 

function [] = compile(self)

	h = self.hash;

	mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
	assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')
	if (isunix && ~ismac)
		warning('off')
	end
	mex('-silent',mexBridge_name,'-outdir',self.xolotl_folder)
	% update linked_binary

	self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];


end