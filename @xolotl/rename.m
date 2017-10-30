%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: renames a already existing compartment 

function rename(self,old_name,new_name)

assert(any(strcmp(old_name,properties(self))),'Unknown compartment') 

assert(self.checkCompartmentName(new_name),'Illegal compartment name')


% copy the old compartment into the new compartment 
self.copy(old_name,new_name)

% figure out where the old compartment is in the list of compartments
old_idx = find(strcmp(old_name,self.compartment_names));
new_comp_names = self.compartment_names;
new_comp_names{old_idx} = new_name;
new_comp_names(end) = [];

% delete the old compartment 
self.removeCompartment(old_name,true);

% move the new name to the location of the old name
self.compartment_names = new_comp_names;

% update all controllers, synapses, etc. 
for i = 1:length(self.synapses)
	if strcmp(self.synapses(i).pre,old_name)
		self.synapses(i).pre = new_name;
	end
	if strcmp(self.synapses(i).post,old_name)
		self.synapses(i).post = new_name;
	end
end

for i = 1:length(self.controllers)
	if strcmp(self.controllers(i).compartment,old_name)
		self.controllers(i).compartment = new_name;
		self.controllers(i).channel = strrep(self.controllers(i).channel,old_name,new_name);
	end

end

self.transpile;