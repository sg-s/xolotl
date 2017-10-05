%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% updates parameters in xolotl object using the argument
% 

function updateLocalParameters(self,parameters)
	% unpack parameters and update locally
	for i = 1:length(parameters)
		if iscell(parameters)
			S = parameters{i};
		else
			S = parameters;
		end
		f = fieldnames(S);
		
		if i <= length(self.compartment_names)

			ff = self.(self.compartment_names{i});
			% match everything we can in this compartment 
			for j = 1:length(f)
				if isfield(self.(self.compartment_names{i}),f{j})
					self.(self.compartment_names{i}).(f{j}) = S.(f{j});
				else
					% maybe one level deeper? 
					cond_name = f{j}(1:strfind(f{j},'_')-1);
					if isfield(self.(self.compartment_names{i}),cond_name)
						self.(self.compartment_names{i}).(cond_name).gbar = S.(f{j});
					end
				end
			end
		else
			% synapses?
			% blindly assume that the order we get them back is the order we have stored locally. dangerous, but it should work 
			v = struct2vec(parameters{i});
			assert(length(v) == length(self.synapses),'Expected this parameter set to be synapse strengths, but this does not have the same length as the synapses I have on list')
			for j = 1:length(self.synapses)
				self.synapses(j).gbar = v(j);
			end
		end
	end
end
