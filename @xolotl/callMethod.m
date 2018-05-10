%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% calls a C++ method before compiling 

% function callMethod(self, cpp_object, method_name, method_values)

% assert(self.exist(cpp_object),'Unknown object. Use a string to describe the object in the c++ tree')
% assert(ischar(method_name),'Method name needs to be a string')

% cpp_object = [strrep(cpp_object,'.','_')];

% if ischar(method_values)
% 	% check if it refers to an object in the tree
% 	assert(self.exist(method_values),'Since method_values is a string, I assume that you are referring to an object in the tree. No such object was found')
% 	method_values = ['&' strrep(method_values,'.','_')];
% end

% if isempty(self.call_method_data)
% 	self.call_method_data = struct('object',cpp_object,'method_name',method_name,'method_values',method_values);


% else
% 	if any(strcmp({self.call_method_data.object},cpp_object) & strcmp({self.call_method_data.method_name},method_name))
% 		error('this method already exists')
% 	end

% 	% add it to the struct
% 	S = struct('object',cpp_object,'method_name',method_name,'method_values',method_values);
% 	self.call_method_data = [self.call_method_data; S];
% end

% % update the hash to include information about call_method_data 
% if isnumeric(method_values)
% 	method_values = num2str(method_values);
% end

% [e,o] = system(['echo "' cpp_object, method_name, method_values self.hash '" | openssl sha1']);
% H = strrep(o,' ','');
% H = strtrim(H);

% if length(H) > 40
% 	H = H(end-39:end);
% elseif length(H) < 40
% 	error('Error determining hash!')
% end

% self.hash = H;