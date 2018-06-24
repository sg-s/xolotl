%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: add thing to model  
%
% example usage:
% x.add('AB',AB)

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

assert(self.checkCompartmentName(obj_name),'Illegal compartment name')



add@cpplab(self,obj, obj_name);