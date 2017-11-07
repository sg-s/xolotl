%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: serializes a xolotl object, discards string values
%  so you can run
% the underlying binary without xolotl
% serialize only coverts numerical values
% strings are discarded 

function [values,names, is_relational] = serialize(self)

% to do -- is-relational not supported for anything except compartment props

values = {};
names = {};
is_relational = {};

% first do the sim params
names{end+1} = {'dt';'t_end'};
values{end+1} = [self.dt; self.t_end];
is_relational{end+1} = [false; false];

% now do all the compartments
for i = 1:length(self.compartment_names)
	[v,n, ir] = struct2vec(self.(self.compartment_names{i}));
	% append compartment name to these
	n = cellfun(@(x) [self.compartment_names{i} x],n,'UniformOutput',false);
	names{end+1} = n;
	values{end+1} = v;
	is_relational{end+1} = ir;
end

% now do the synapses
if length(self.synapses)> 0
	S = {}; SV = [];
	for i = 1:length(self.synapses)
		S{end+1} = [self.synapses(i).pre '_2_' self.synapses(i).post '_' self.synapses(i).type(1:4) '_gbar'];
		SV(end+1) = funeval(self.synapses(i).gbar);
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
elseif ~isempty(self.I_ext)
	names{end+1} = 'I_ext';
	values{end+1} = self.I_ext;
else
	names{end+1} = '';
	values{end+1} = [];
end


% now do the controllers
if length(self.controllers)> 0
	S = {}; SV = [];
	for i = 1:length(self.controllers)
		this_controller = self.controllers{i};
		fn = fieldnames(this_controller);
		for j = 1:length(fn)
			if ~strcmp(fn{j},'type') && isnumeric(this_controller.(fn{j}))
				S{end+1} = [this_controller.channel '_' this_controller.type '_' fn{j}];
				SV(end+1) = this_controller.(fn{j});
			end
		end
	end
	names{end+1} = S;
	values{end+1} = SV;
else
	names{end+1} = '';
	values{end+1} = [];
end
