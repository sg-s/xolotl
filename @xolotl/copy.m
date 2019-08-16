
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### copy
%
% Syntax:
%
% ```matlab
% x2 = copy(x);
% ```
%
% copies a xolotl object. ``copy`` creates an identical
% copy of a xolotl object that can be manipulated separately.
% Both copies will use the same binary to integrate,
% unless you add a new component to one of them.
%
% !!! warning
% XXXX* Some read-only properties in a xolotl object may not be copied over.
% XXXX* Do not make vectors of ``xolotl`` objects, as it may lead to undefined behavior.
%
%
% See Also: 
% cpplab.copy
%     * [How to copy models](https://xolotl.readthedocs.io/en/master/how-to/copy-models/)
% 



function N = copy(self)

% make a new cpplab object
N = xolotl;

C = self.Children;
N.Children = {};

% copy cpplab properties by recursively calling copy
for i = 1:length(C)
	NN = self.(C{i}).copy;
	N.add(NN,C{i});
end

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


% synchronize states
N.deserialize(self.serialize)
