
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### slice
%
% **Syntax**
%
% ```matlab
% % assuming there is a compartment called 'Dendrite'
% x.slice('Dendrite',10)
% ```
%
% **Description**
%
% `slice` partitions a cylindrical compartment into N slices.
%
% The compartment to be sliced must explicitly be a cylindrical
% section, i.e., it must have a defined length and radius.
% `slice` cuts the cylinder along the axis, and connects each
% slice with `Axial` synapses. This object can then be treated
% as a multi-compartment model, and `xolotl` will integrate
% it using the Crank-Nicholson scheme reserved for multi-compartment models.
%
%
% See Also: 
% xolotl.connect



function slice(self, compartment, N_slices, axial_resistivity)

corelib.assert(any(strcmp(self.find('compartment'),compartment)),'Unknown compartment')


corelib.assert(~any(N_slices - floor(N_slices)),'N_slices must be an integer > 1')
corelib.assert(isscalar(N_slices),'N_slices must be an integer > 1')
corelib.assert(N_slices > 1,'N_slices must be an integer > 1')

if nargin < 4
	axial_resistivity = NaN;
end

corelib.assert(isscalar(axial_resistivity),'axial_resistivity must be a real +ve number')
if ~isnan(axial_resistivity)
	corelib.assert(axial_resistivity > 0,'axial_resistivity must be a real +ve number')
end

if iscell(compartment)
	for i = 1:length(compartment)
		self.slice(compartment{i},N_slices,axial_resistivity)
	end
end

% TODO check that there are no incident synapses
comp_children = self.(compartment).Children;
for i = 1:length(comp_children)
	if strcmp(self.(compartment).(comp_children{i}).cpp_class_parent,'synapse')
		error('There are incident synapses onto this compartment; cannot slice')
	end
end



% we assume cylindrical geometry
% so make sure that the radius and length
% fields are filled out

corelib.assert(~isnan(self.(compartment).radius),'Radius of compartment must be specified')
corelib.assert(~isnan(self.(compartment).len),'Length of compartment must be specified')


new_len = self.(compartment).len/N_slices;

self.(compartment).len = new_len;

all_comps = {compartment};

n_digits = length(mat2str(N_slices));

compartment_root_name = strrep(compartment,strjoin(regexp(compartment,'[0-9]','match'),''),'');

root_comp = copy(self.(compartment));

% destroy the original compartment
self.(compartment).destroy();
all_comps = {};



for i = 1:N_slices

	padding_length = n_digits - length(mat2str(i));
	new_comp_name = [compartment_root_name repmat('0',1,padding_length) mat2str(i)];


	self.add(copy(root_comp), new_comp_name);
	all_comps = [all_comps; new_comp_name];
end


% wire them up with Axial objects


for i = 2:N_slices
	synapse = cpplab('Axial','resistivity',axial_resistivity);

	self.connect(all_comps{i-1},all_comps{i},copy(synapse))
	self.connect(all_comps{i},all_comps{i-1},copy(synapse))

end
