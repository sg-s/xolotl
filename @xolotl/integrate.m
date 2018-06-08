%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: integrates the model
%

function [V, Ca, cont_state, curr_state, syn_state] = integrate(self)


if isempty(self.linked_binary)
	h = self.hash;
	mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
	self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];
end


% does the binary exist?
if exist(joinPath(self.xolotl_folder,self.linked_binary),'file') == 3
	% does the hash match up?
	h = self.hash;
	if ~strcmp(self.linked_binary(10:15),h(1:6))
		self.transpile;
		self.compile;
	end

else
	self.transpile;
	self.compile;
end

% check that sim_dt and output_dt make sense
if isnan(self.sim_dt) || isempty(self.sim_dt)
	self.sim_dt = self.dt;
end
assert(isint(self.dt/self.sim_dt),'Simulation & output dt are not compatible')

if nargout == 0 & self.closed_loop == false
	error('Are you sure you want to integrate this with no outputs and with closed_loop set to FALSE?')
end

V = [];
Ca = [];
I_clamp = [];
curr_state = [];
syn_state = [];
cont_state = [];


n_comp = length(self.find('compartment'));
n_steps = floor(self.t_end/self.sim_dt);


% make sure I_ext and V_clamp are specified
if isempty(self.V_clamp)
	% fill with NaNs
	self.V_clamp = NaN(n_steps,n_comp);
end

if isempty(self.I_ext)
	% fill with zeros
	self.I_ext = zeros(n_steps,n_comp);
end


% vectorize the current state
arguments = self.serialize;

n_comp = length(self.find('compartment'));
n_steps = floor(self.t_end/self.sim_dt);


[~,f] = fileparts(self.linked_binary);

f = str2func(f);
[results{1:nargout+1}] = f(arguments,self.I_ext,self.V_clamp);

if self.closed_loop
	self.deserialize(results{1});
end

if nargout > 0
	V = (results{2})';
end
if nargout > 1
	Ca = (results{3})';
end
if nargout > 2
	cont_state = (results{4})';
end
if nargout > 3
	curr_state = (results{5})';
end
if nargout > 4
	syn_state = (results{6})';
end
