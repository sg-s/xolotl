%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% copy
% ====
%
% copies a xolotl object. ``copy`` creates an identical copy of a xolotl object that can be manipulated seperately. Both copies will use the same binary to integrate, unless you add a new component to one of them. Syntax ::
%
%   x2 = copy(x);
%
% .. warning::
%
%   Some read-only properties in a xolotl object may not be copied over. 

function N = copy(self)

% make a new cpplab object
N = xolotl;

% copy over every non-cpplab property
props = properties(self);
for i = 1:length(props)
	if isa(self.(props{i}),'cpplab')
		continue
	end
	try
		N.(props{i}) = self.(props{i});
	catch
	end
end

% copy cpplab propoerties by recurisvely calling copy
C = self.Children;
for i = 1:length(C)
	NN = self.(C{i}).copy;
	N.add(NN,C{i});
end

% synchronize states
N.deserialize(self.serialize)