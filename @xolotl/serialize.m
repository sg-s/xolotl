%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% serializes a xolotl object so you can run
% the underlying binary without xolotl 

function [values,names] = serialize(self)

values = {};
names = {};

% first do the sim params
names{end+1} = {'dt';'t_end'};
values{end+1} = [self.dt; self.t_end];

% now do all the compartments 
for i = 1:length(self.compartment_names)
	[v,n] = struct2vec(self.(self.compartment_names{i}));
	% append compartment name to these 
	n = cellfun(@(x) [self.compartment_names{i} x],n,'UniformOutput',false);
	names{end+1} = n;
	values{end+1} = v;
end

% now do the synapses 
if length(self.synapses)> 0
	S = {}; SV = [];
	for i = 1:length(self.synapses)
		S{end+1} = [self.synapses(i).pre '_2_' self.synapses(i).post '_' self.synapses(i).type(1:4) '_gbar'];
		SV(end+1) = self.synapses(i).gbar;
		S{end+1} = [self.synapses(i).pre '_2_' self.synapses(i).post '_' self.synapses(i).type(1:4) '_s'];
		SV(end+1) = self.synapses(i).state;
	end
	names{end+1} = S;
	values{end+1} = SV;
else
	names{end+1} = '';
	values{end+1} = [];
end

% now do V_clamp
if ~isempty(self.V_clamp)
	names{end+1} = 'V_clamp';
	values{end+1} = self.V_clamp;
else
	names{end+1} = '';
	values{end+1} = [];
end


% now do the controllers
if length(self.controllers)> 0
	S = {}; SV = [];
	for i = 1:length(self.controllers)
		S{end+1} = [self.controllers(i).channel '_controller_tau_m'];
		SV(end+1) = self.controllers(i).tau_m;
		S{end+1} = [self.controllers(i).channel '_controller_tau_g'];
		SV(end+1) = self.controllers(i).tau_g;
		S{end+1} = [self.controllers(i).channel '_controller_Alpha'];
		SV(end+1) = self.controllers(i).Alpha;
		S{end+1} = [self.controllers(i).channel '_controller_m'];
		SV(end+1) = self.controllers(i).m;
		
	end
	names{end+1} = S;
	values{end+1} = SV;
else
	names{end+1} = '';
	values{end+1} = [];
end
