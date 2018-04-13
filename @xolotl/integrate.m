%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: integrates the model
% 

function [V, Ca,I_clamp, cond_state, syn_state, cont_state] = integrate(self,I_ext, V_clamp)

if nargin == 2
	assert(length(I_ext) == length(self.find('compartment')),'I_ext should be a vector with an element for each compartment')
end

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
cond_state = [];
syn_state = [];
cont_state = [];



% vectorize the current state 
arguments = self.serialize;

n_comp = length(self.find('compartment'));
n_steps = floor(self.t_end/self.sim_dt);

switch nargin 
case 1
	I_ext = zeros(n_comp,1);
	V_clamp = NaN(n_comp,n_steps);
case 2
	% only I_ext
	V_clamp = NaN(n_comp,n_steps);
case 3
	% ignore I_ext, since it's being clamped
	I_ext = zeros(n_comp,1);	
	if length(V_clamp) == n_comp
		V_clamp = repmat(V_clamp,1,n_steps);
	end
end

assert(length(I_ext) == n_comp,'Size of I_ext is incorrect')
assert(size(V_clamp,1) == n_comp,'Size of V_clamp is incorrect')
assert(size(V_clamp,2) == n_steps,'Size of V_clamp is incorrect')

[~,f] = fileparts(self.linked_binary);

f = str2func(f);
[results{1:nargout+1}] = f(arguments,I_ext,V_clamp);


if self.closed_loop
	self.deserialize(results{1});
end

if nargout > 0
	V = (results{2})';
end
if nargout > 1
	Ca = (results{3})';
	% cond_state = (results{5})';
	% syn_state = (results{6})';
	% cont_state = (results{7})';
end

