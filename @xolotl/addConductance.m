%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: add conductances to a compartment 
% cond_id should be a string that matches some
% conductance header file in conductances/

function addConductance(self,compartment,cond_id,gbar,E,m,h,Q_g, Q_tau_m, Q_tau_h)

% dumb windows backslashes
cond_id = strrep(cond_id,'/',filesep);
cond_id = strrep(cond_id,'\',filesep);

assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

% search for cond_id
cond_file = [];
for i = 1:length(self.available_conductances)
	if any(strfind(self.available_conductances{i},cond_id))
		cond_file = i;
		break;
	end
end
assert(~isempty(cond_file),'Which conductance do you mean?')

if nargin < 6
	m = 0;
	h = 1;
end
if nargin < 7
	h = 1;
end
if nargin < 10
	Q_g = 1;
	Q_tau_m = 1;
	Q_tau_h = 1;
end

cond_name = pathEnd(self.available_conductances{cond_file});


full_path = strrep(self.available_conductances{cond_file},self.xolotl_folder,'');
if strcmp(full_path(1),filesep)
	full_path(1) = [];
end

self.(compartment).(cond_name).full_path = full_path;
self.(compartment).(cond_name).gbar = gbar;
self.(compartment).(cond_name).E = E;
self.(compartment).(cond_name).m = m;
self.(compartment).(cond_name).h = h;

self.(compartment).(cond_name).Q_g = Q_g;
self.(compartment).(cond_name).Q_tau_m = Q_tau_m;
self.(compartment).(cond_name).Q_tau_h = Q_tau_h;

% add this to conductance_headers, if it's not already there

self.conductance_headers = [self.conductance_headers; full_path];
