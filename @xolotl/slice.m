%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: divides a compartment into slices
function slice(self, compartment, N_slices, g_axial)

assert(any(strcmp(self.find('compartment'),compartment)),'Unknown compartment')
assert(isint(N_slices),'N_slices must be an integer > 1')
assert(isscalar(N_slices),'N_slices must be an integer > 1')
assert(N_slices > 1,'N_slices must be an integer > 1')
assert(isscalar(g_axial),'g_axial must be a real +ve number')
assert(g_axial > 0,'g_axial must be a real +ve number')

% TODO check that there are no incident synapses

if iscell(compartment)
	for i = 1:length(compartment)
		self.slice(compartment{i},N_slices,g_axial)
	end
end


new_vol = self.(compartment).vol/N_slices;
new_A = self.(compartment).A/N_slices;

self.(compartment).vol = new_vol;
self.(compartment).A = new_A;

all_comps = {compartment};

n_digits = length(mat2str(N_slices));

for i = 2:N_slices
	root_comp = copy(self.(compartment));

	padding_length = n_digits - length(mat2str(i));
	new_comp_name = [compartment repmat('0',1,padding_length) mat2str(i)];

	self.add(new_comp_name,root_comp);
	all_comps = [all_comps; new_comp_name];
end

% wire them up with electrical synapses 
for i = 2:N_slices
	self.connect(all_comps{i-1},all_comps{i},g_axial);
end

