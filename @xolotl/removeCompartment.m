%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% remove compartment from model 

function removeCompartment(self,label)


	% TO DO -- check that there are no synapses connecting to this compartment before removing them
	assert(~any(strcmp(label,{self.synapses.post})),'At least one synapse connects to this compartment. Remove the synapse before deleting this compartment')
	assert(~any(strcmp(label,{self.synapses.pre})),'At least one synapse connects to this compartment. Remove the synapse before deleting this compartment')

	assert(any(strcmp(label,{self.dyn_prop_handles.Name})),'Could not find the compartment you want to remove')

	comp_to_remove = find(strcmp(label,{self.dyn_prop_handles.Name}));
	delete(self.dyn_prop_handles(comp_to_remove));
	self.dyn_prop_handles(comp_to_remove) = [];

	% remove this name from the list of compartment names
	self.compartment_names(strcmp(label,self.compartment_names)) = [];

end