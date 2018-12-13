%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

# connect

Connects two compartments with a synapse. The basic syntax is ::

   x.connect('Comp1', 'Comp2', 'SynapseType', ...)

The first two arguments are the presynaptic and postsynaptic compartment names. For example ::

```
% connects two different neurons with an electrical synapse
x.connect('AB', 'LP')
```

Axial synapses are a special type of electrical synapse that are created between spatially-discrete compartments in a morphological structure. Electrical and axial synapses differ in how they are integrated (see Dayan & Abbott 2001, Ch. 5-6).

`connect` defaults to an axial synapse when the type of synapse is not specified and either compartment has a defined `tree_idx` (which identifies the compartment as a part of a multi-compartment neuron model). Otherwise, the created synapse is electrical. ::


```
% create an (electrical or axial) synapse between AB and LP with gbar f NaN
x.connect('AB', 'LP')
% create an (electrical or axial) synapse between AB and LP with gbar f 10
x.connect('AB', 'LP', 10)
```

The most common way to produce a synapse is to pass the synapse type and hen any properties. This is used to create chemical synapses. For example, o add a glutamatergic synapse (from Prinz *et al.* 2004) between `AB` and `LP` with a maximal conductance of 100: ::

```
x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)
```

Synapses can also be connected by passing a `cpplab` object to the `connect` method ::

```
% create a synapse using the cpplab object 'syn_cpplab' 
x.connect('AB', 'LP', syn_cpplab)
```

The following properties can be specified

======================= ================
Name                    PropertyName
Maximal conductance     `gmax`
Reversal potential      `E`
Activation variable     `s`
======================= ================

%}

function connect(self,comp1,comp2,varargin)

assert(ischar(comp1),'First argument should be a string; the name of the presynaptic compartment')
assert(ischar(comp2),'Second argument should be a string; the name of the postsynaptic compartment')
assert(any(strcmp(comp1,properties(self))),'Unknown compartment')
assert(any(strcmp(comp2,properties(self))),'Unknown compartment')

assert(~strcmp(comp1,comp2),'You cannot connect a compartment to itself')





if length(varargin) == 1 && isa(varargin{1},'cpplab')
	% we are given an object. blindly use it
	synapse = varargin{1};
	self.(comp2).add([synapse.cpp_class_name comp1],synapse);

	this_syn_info = struct;
	this_syn_info.pre_synapse = comp1;
	this_syn_info.post_synapse = comp2;
	this_syn_info.syn_name = [comp2 '.' synapse.cpp_class_name comp1];

	if isempty(self.synapses)
		self.synapses = this_syn_info;
	else
		self.synapses = [self.synapses; this_syn_info];
	end

elseif length(varargin) > 1

	synapse = cpplab(varargin{:});


	self.(comp2).add([synapse.cpp_class_name comp1],synapse);

	this_syn_info = struct;
	this_syn_info.pre_synapse = comp1;
	this_syn_info.post_synapse = comp2;
	this_syn_info.syn_name = [comp2 '.' synapse.cpp_class_name comp1];

	if isempty(self.synapses)
		self.synapses = this_syn_info;
	else
		self.synapses = [self.synapses; this_syn_info];
	end

elseif length(varargin) == 0
	% no synapse type specified, use Axial synapses to connect
	synapse = cpplab('Axial');

	self.connect(comp1,comp2,copy(synapse))
	self.connect(comp2,comp1,copy(synapse))

else
	error('Unknown argument type')

end

