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
assert(self.checkCompartmentName(obj_name),'Illegal compartment name')

obj = cpplab(obj_type, varargin{:});

add@cpplab(self,obj, obj_name);