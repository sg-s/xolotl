%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: add a synapse between two compartments
% syn_id should be a string that matches some synapse
% comp1 and comp2 should be strings that match existing compartments 

function addSynapse(self,syn_id,comp1,comp2,gbar)

% dumb windows backslashes
syn_id = strrep(syn_id,'/',filesep);
syn_id = strrep(syn_id,'\',filesep);

% fail early 
assert(any(strcmp(comp1,self.compartment_names)),'Unknown compartment')
assert(any(strcmp(comp2,self.compartment_names)),'Unknown compartment')
assert(length(gbar) == 1,'gbar has the wrong size')
assert(~isnan(gbar),'gbar cannot be NaN')

% search for syn_id
syn_file = [];
for i = 1:length(self.available_synapses)
	if any(strfind(self.available_synapses{i},syn_id))
		syn_file = i;
		break;
	end
end
assert(~isempty(syn_file),'Which synapse do you mean?')

syn_name = pathEnd(self.available_synapses{syn_file});

S.type = syn_name;
S.pre = comp1;
S.post  = comp2;
S.gbar = gbar;
S.state = 0;

self.synapses  = [self.synapses; S];

% add this to synapse_headers, if it's not already there
add_this = strrep(self.available_synapses{syn_file},self.xolotl_folder,'');
if strcmp(add_this(1),filesep)
	add_this(1) = [];
end

self.synapse_headers = [self.synapse_headers; add_this];
