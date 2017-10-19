%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% slice an existing compartment into N sections, with
% elctrical synapses b/w them with strength gbar
% all previously existing conductances are preserved 

function slice(self,compartment,N,gbar)
	assert(any(strcmp(compartment,properties(self))),'Unknown compartment')
	assert(isscalar(N),'the second argument should be the number of slices you want; a scalar')
	assert(N>1,'the number of slices you want should be > 1')


	% make sure this compartment has no chemical synapses on it
	if ~isempty(self.synapses)
		assert(any(strcmp(compartment,{self.synapses.pre}) & ~strcmp('Electrical',{self.synapses.type})),'At least one non-electrical synapse on this compartment. Cannot be sliced')
		assert(any(strcmp(compartment,{self.synapses.post}) & ~strcmp('Electrical',{self.synapses.type})),'At least one non-electrical synapse on this compartment. Cannot be sliced')
	end

	% to do -- 1 electrical synapse

	% to do -- 2 electrical synapses 

	% resize the compartment 
	new_area = self.(compartment).A/N;
	new_vol = self.(compartment).vol/N;
	self.(compartment).vol = new_vol;
	self.(compartment).A = new_area;

	% rename the compartment 

	% copy the compartment n-1 times
	for i = 2:N
		new_name = [compartment 'S' mat2str(i)];
		self.copy(compartment,new_name);
	end

	% add synapses to wire them together 
	

end