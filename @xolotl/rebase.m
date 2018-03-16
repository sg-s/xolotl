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

	% make sure cpplab knows where the code is
	cpplab.addPath(joinPath(fileparts(fileparts(which(mfilename))),'c++'))

	% make a list of the available conductances
	available_conductances = getAllFiles(joinPath(self.cpp_folder,'conductances'));
	rm_this = true(length(available_conductances),1);
	for i = 1:length(available_conductances)
		[~,cond_name,ext] = fileparts(available_conductances{i});
		if strcmp(ext,'.hpp')
			self.illegal_names{end+1} = cond_name;
			rm_this(i) = false;
		end
	end
	self.available_conductances = available_conductances(~rm_this);

	% make a list of available controllers
	available_controllers = getAllFiles(joinPath(self.cpp_folder,'controllers'));
	rm_this = true(length(available_controllers),1);
	for i = 1:length(available_controllers)
		[~,cont_name,ext] = fileparts(available_controllers{i});
		if strcmp(ext,'.hpp')
			self.illegal_names{end+1} = cont_name;
			rm_this(i) = false;
		end
	end
	self.available_controllers = available_controllers(~rm_this);


	% make a list of the available synapses
	available_synapses = getAllFiles(joinPath(self.cpp_folder,'synapses'));
	rm_this = true(length(available_synapses),1);
	for i = 1:length(available_synapses)
		[~,syn_name,ext] = fileparts(available_synapses{i});
		if strcmp(ext,'.hpp')
			self.illegal_names{end+1} = syn_name;
			rm_this(i) = false;
		end
	end
	self.available_synapses = available_synapses(~rm_this);

	if ismac
		self.OS_binary_ext = 'mexmaci64';
	elseif ispc 
		self.OS_binary_ext = 'mexw64';
	else
		self.OS_binary_ext = 'mexa64';
	end


end