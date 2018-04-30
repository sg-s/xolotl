%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: connects two compartments with a synapse

function connect(self,comp1,comp2,varargin)

assert(ischar(comp1),'First argument should be a string; the name of the presynaptic compartment')
assert(ischar(comp2),'Second argument should be a string; the name of the postsynaptic compartment')
assert(any(strcmp(comp1,properties(self))),'Unknown compartment')
assert(any(strcmp(comp2,properties(self))),'Unknown compartment')


if isempty(self.synapse_pre)
	self.synapse_pre = {};
end
if isempty(self.synapse_post)
	self.synapse_post = {};
end

if isempty(varargin)
	
	error('Need to specify how to connect these compartments')
elseif length(varargin) == 1 && isdouble(varargin{1})
	% default to a electrical synapse
	synapse = cpplab('Electrical','gbar',varargin{1});
	
	% need to add it twice because each electrical
	% synapse is actually one way
	self.synapses = [self.synapses; synapse; copy(synapse)];

	self.synapse_pre = [self.synapse_pre; comp1];
	self.synapse_post = [self.synapse_post; comp2];

	self.synapse_pre = [self.synapse_pre; comp2];
	self.synapse_post = [self.synapse_post; comp1];

elseif length(varargin) == 1 && isa(varargin{1},'cpplab')
	% we are given an object. blindly use it
	synapse = varargin{1};
	self.synapses = [self.synapses; synapse];

	self.synapse_pre = [self.synapse_pre; comp1];
	self.synapse_post = [self.synapse_post; comp2];

else

	synapse = cpplab(varargin{:});
	self.synapses = [self.synapses; synapse];

	self.synapse_pre = [self.synapse_pre; comp1];
	self.synapse_post = [self.synapse_post; comp2];


end

% because these objects are added within a function,
% we need to update the hash

self.sha1hash;
