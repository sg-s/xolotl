%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: divides a compartment into slices
% used only in multi-compartment models
% compartments are connected with a special 
% synapse called "Axial"

function slice(self, compartment, N_slices, axial_resistivity)

assert(any(strcmp(self.find('compartment'),compartment)),'Unknown compartment')
assert(isint(N_slices),'N_slices must be an integer > 1')
assert(isscalar(N_slices),'N_slices must be an integer > 1')
assert(N_slices > 1,'N_slices must be an integer > 1')

if nargin < 4
	axial_resistivity = NaN;
end

assert(isscalar(axial_resistivity),'axial_resistivity must be a real +ve number')
if ~isnan(axial_resistivity)
	assert(axial_resistivity > 0,'axial_resistivity must be a real +ve number')
end

% TODO check that there are no incident synapses

if iscell(compartment)
	for i = 1:length(compartment)
		self.slice(compartment{i},N_slices,axial_resistivity)
	end
end


% we assume cylindrical geometry
% so make sure that the radius and length
% fields are filled out 

assert(~isnan(self.(compartment).radius),'Radius of compartment must be specified')
assert(~isnan(self.(compartment).len),'Length of compartment must be specified')

skip_hash_state = self.skip_hash;
self.skip_hash = true;

new_len = self.(compartment).len/N_slices;

self.(compartment).len = new_len;

all_comps = {compartment};

n_digits = length(mat2str(N_slices));

compartment_root_name = strrep(compartment,strjoin(regexp(compartment,'[0-9]','match'),''),'');

for i = 2:N_slices
	root_comp = copy(self.(compartment));

	padding_length = n_digits - length(mat2str(i));
	new_comp_name = [compartment_root_name repmat('0',1,padding_length) mat2str(i)];


	self.add(root_comp, new_comp_name);
	all_comps = [all_comps; new_comp_name];
end

% wire them up with Axial objects 

if isempty(self.synapse_pre)
	self.synapse_pre = {};
end
if isempty(self.synapse_post)
	self.synapse_post = {};
end

for i = 2:N_slices

	synapse = cpplab('Axial','resistivity',axial_resistivity);
	
	% need to add it twice -- once each way
	self.synapses = [self.synapses; synapse; copy(synapse)];

	self.synapse_pre = [self.synapse_pre; all_comps{i-1}];
	self.synapse_post = [self.synapse_post; all_comps{i}];

	self.synapse_pre = [self.synapse_pre; all_comps{i}];
	self.synapse_post = [self.synapse_post; all_comps{i-1}];

end

self.skip_hash = false;
self.md5hash;
self.skip_hash = skip_hash_state;