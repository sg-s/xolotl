%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% connects two compartments via an electrical
% synapse

function connect(self,comp1,comp2,gbar)

assert(any(strcmp(comp1,properties(self))),'Unknown compartment')
assert(any(strcmp(comp2,properties(self))),'Unknown compartment')
assert(isscalar(gbar),'gbar should be a scalar')
assert(gbar>0,'gbar should be positive')

self.addSynapse('Elec',comp1,comp2,gbar);
self.addSynapse('Elec',comp2,comp1,gbar);