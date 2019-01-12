%{
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### add


**Syntax**

```matlab
x.add('compartment','comp_name')
x.add(compartment,'comp_name')
x.add('compartment','comp_name',...)
```

**Description**

Adds a `cpplab` object to a `xolotl` object. The `add` method is the most important way you construct models. 

- **`x.add('compartment','comp_name')`** adds a compartment to the xolotl object and names it `comp_name`.

- **`x.add(compartment,'comp_name')`** adds a compartment object (a cpplab object) to the xolotl object `x` and names it `comp_name`. Note that compartment is a cpplab object sourced from the `compartment.hpp` C++ file, and can contain children and be extensively modified. 

- **`x.add('compartment','comp_name',...)`** adds a compartment to the xolotl object and names it `comp_name`. The compartment is then additionally configured using the parameters specified using Name Value syntax. 

**Technical Details**

`xolotl.add` checks that the compartment being added has a legal name using `checkCompartmentName`. If so, it calls the `add` method in the `cpplab` superclass. 

!!! info "See Also"
    ->cpplab.add
    ->xolotl.checkCompartmentName

%}

function add(self,obj_type, obj_name,varargin)

msg = ['You tried to add a compartment without a label, which is not allowed. Every compartment must be labeled. \n\n<a href="https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#add">Click here to read the documentation on how to use this method</a>'];
assert(nargin > 2,'xolotl:add:no_label',msg)

msg = ['The name of the object you are trying to add must be a character vector. \n\n<a href="https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#add">Click here to read the documentation on how to use this method</a>'];
assert(ischar(obj_name),'xolotl:add:name_type_mismatch',msg)

msg = ['Illegal compartment name: ' obj_name '\n\n<a href="https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#add">Click here to read the documentation on how to use this method</a>'];
assert(self.checkCompartmentName(obj_name),'xolotl:add:illegal_name',msg)

if isa(obj_type,'cpplab')
	% we're adding a cpplab object
	% if so, then the convention is 
	% self.add(obj, obj_name)


	msg = ['You are attempting to add a cpplab object to a xolotl object. To do so, the convention is "self.add(obj, obj_name)". \n\n<a href="https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#add">Click here to read the documentation on how to use this method</a>'];

	assert(length(varargin) == 0,'xolotl:add:cpplab_object' ,msg)

	obj = obj_type;
	
else
	
	obj = cpplab(obj_type, varargin{:});

end


% object should be a compartment object
assert(strcmp(obj.cpp_class_name,'compartment'),'xolotl:add:not_compartment' ,'You can only add objects of type "compartment" to a xolotl object')


add@cpplab(self,obj, obj_name);

% reset I_ext and V_clamp
N = length(self.Children);
self.I_ext = zeros(1,N);
self.V_clamp = NaN(1,N);