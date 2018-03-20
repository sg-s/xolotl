
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% marks an item as being contained in another
% useful for custom objection construction
%  
function put(self, thing, owner)

if iscell(thing)
	for i = 1:length(thing)
		self.put(thing{i},owner);
	end
	return
end

% check that this thing exists
assert(self.exist(thing),'Object does not exist')
assert(self.exist(owner),'Owner does not exist')

self.custom_owner = [self.custom_owner; {thing owner}];