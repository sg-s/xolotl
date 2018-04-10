%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% sets the xolotl_folder, cpp_folder,
% and searches these folders to build 
% a list of full paths for all conductances,
% controllers, synapses, etc. 

function [] = rebase(self)

	self.xolotl_folder = fileparts(fileparts(which(mfilename)));
	self.cpp_folder = joinPath(self.xolotl_folder,'c++');

	if ismac
		self.OS_binary_ext = 'mexmaci64';
	elseif ispc 
		self.OS_binary_ext = 'mexw64';
	else
		self.OS_binary_ext = 'mexa64';
	end


end