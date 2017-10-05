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
	values{end+1} = [self.synapses.gbar];
	these_names = cell(length(self.synapses),1);
	for i = 1:length(self.synapses)
		these_names{i} = [self.synapses(i).pre '_2_' self.synapses(i).post '_' self.synapses(i).type(1:4)];
	end
	names{end+1} = these_names;
else
	names{end+1} = '';
	values{end+1} = [];
end