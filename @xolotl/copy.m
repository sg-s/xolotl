%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: copy a compartment, with all conductances 

function copy(self,compartment,copy_name)
	assert(any(strcmp(compartment,properties(self))),'Unknown compartment')
	assert(self.checkCompartmentName(copy_name),'Illegal copy name')

	self.dyn_prop_handles = [self.dyn_prop_handles ;self.addprop(copy_name)];
	self.compartment_names = [self.compartment_names; copy_name];

	self.(copy_name) = self.(compartment);

end