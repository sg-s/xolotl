%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### connect


**Syntax**

```matlab
x.connect('Comp1', 'Comp2')
x.connect('Comp1', 'Comp2',resistivity)
x.connect('Comp1', 'Comp2', SynapseObj)
x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp')
x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp','Parameter',Value...)
```

**Description**

Connects two compartments with a synapse. 


- **`x.connect('Comp1', 'Comp2')`** connects two compartments, `Comp1` and `Comp2`, using reciprocal `Axial` synapses. 


- **`x.connect('Comp1', 'Comp2',resistivity)`** connects two compartments, `Comp1` and `Comp2`, using reciprocal `Axial` synapses and specifies the axial resistivity. 

- **`x.connect('Comp1', 'Comp2', SynapseObj)`** makes a synapse with presynaptic compartment `Comp1` and post-synaptic compartment `Comp2` using the synapse object `SynapseObj`. SynapseObj is a cpplab object that corresponds to a synapse. 

- **`x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp')`** makes a synapse with presynaptic compartment `Comp1` and post-synaptic compartment `Comp2` using a synapse object that is generated on the fly using the C++ header file specified by `'path/to/synapse.hpp'`


- **`x.connect('Comp1', 'Comp2', 'path/to/synapse.hpp','Parameter',Value...)`** makes a synapse with presynaptic compartment `Comp1` and post-synaptic compartment `Comp2` using a synapse object that is generated on the fly using the C++ header file specified by `'path/to/synapse.hpp'`, additionally configuring that object with parameters and values using name-value notation. 


The following properties can be specified for most synapses:

| Name                 |   PropertyName |
|----------------------| -----------| 
| Maximal conductance  |  `gmax`|
| Reversal potential   |  `E`|
| Activation variable  |  `s`|

This method supports tab completion. You should be able to press
tab to get a list of compartments to connect. 

%}

function connect(self,comp1,comp2,varargin)

assert(ischar(comp1),'First argument should be a string; the name of the presynaptic compartment')
assert(ischar(comp2),'Second argument should be a string; the name of the postsynaptic compartment')
assert(any(strcmp(comp1,self.Children)),['Unknown compartment: ' comp1])
assert(any(strcmp(comp2,self.Children)),['Unknown compartment: ' comp2])

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

elseif length(varargin) == 1 && isa(varargin{1},'char')
	thing = cpplab(varargin{1});
	self.connect(comp1,comp2,thing);

elseif length(varargin) == 1 && isa(varargin{1},'double')

	% no synapse type specified, use Axial synapses to connect
	synapse = cpplab('Axial');
	synapse.resistivity = varargin{1};

	self.connect(comp1,comp2,copy(synapse))
	self.connect(comp2,comp1,copy(synapse))

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

