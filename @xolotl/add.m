%{
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

# add

adds a `cpplab` object to a `xolotl` object.

The add method is the most important way you construct models. 

Usage:

```matlab
x.add(compartment,'comp_name')
x.add('compartment','comp_name')
x.add('compartment','comp_name',...)
```

There are two primary ways of using `add`. The first is to 
first construct a `cpplab` object (here called AB), and then
 add it to the `xolotl` object using `x.add(AB,'AB')`. 
`xolotl` requires that every compartment is named, and 
the name has to be specified as a string argument. 


%}

function add(self,obj_type, obj_name,varargin)

assert(nargin > 2,'No label! You have to label every compartment')
assert(ischar(obj_name),'The name of the object has to be a character vector')
if isa(obj_type,'cpplab')
	% we're adding a cpplab object
	% if so, then the convention is 
	% self.add(obj, obj_name)

	assert(length(varargin) == 0, 'You are attempting to add a cpplab object to a xolotl object. To do so, the convention is "self.add(obj, obj_name)" ')

	obj = obj_type;
	
else
	
	obj = cpplab(obj_type, varargin{:});

end

assert(self.checkCompartmentName(obj_name),['Illegal compartment name: ' obj_name])

add@cpplab(self,obj, obj_name);

% reset I_ext and V_clamp
N = length(self.find('compartment'));
self.I_ext = zeros(1,N);
self.V_clamp = NaN(1,N);