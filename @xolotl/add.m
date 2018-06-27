%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% add
% ===
%
% adds a cpplab object to a xolotl object.
%
% The add method is the most important way you construct models. Usage ::
%
%   x.add(compartment,'comp_name')
%   x.add('compartment','comp_name')
%   x.add('compartment','comp_name',...)
%
% There are two primary ways of using ``add``. The first is to first construct a ``cpplab`` object (here called AB), and then add it to the xolotl object using x.add(AB,'AB'). xolotl requires that every compartment is named, and the name has to be specified as a string argument. 
% 
% .. warning::
%
%   Some read-only properties in a xolotl object may not be copied over. 

function add(self,obj_type, obj_name,varargin)

assert(nargin > 2,'No label! You have to label every compartment')

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