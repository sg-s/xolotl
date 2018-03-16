%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% 
% help: check that compartment name is legal

function ok = checkCompartmentName(self,comp_name)

ok = false;

% check that the name doesn't conflict with the list of illegal names
if any(strcmp(comp_name,self.illegal_names))
	return
end

ok = true;

