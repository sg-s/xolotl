%                    _       _     
%   ___  _     _    | | __ _| |__  
%  / __|| |_ _| |_  | |/ _` | '_ \ 
% | (_|_   _|_   _| | | (_| | |_) |
%  \___||_|   |_|   |_|\__,_|_.__/ 
%
% a copy method that actually works,
% unlike MATLAB's built in copy

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