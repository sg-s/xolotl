%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

integrate
^^^^^^^^^

integrates a ``xolotl`` model. Usage ::

   V = x.integrate;
   I_clamp = x.integrate;
   [V, Ca] = x.integrate;
   [V, Ca, mech_state] = x.integrate;
   [V, Ca, mech_state, I] = x.integrate;
   [V, Ca, mech_state, I, syn_state] = x.integrate;


``integrate`` will return different outputs as shown above. Unless you need every output, it is recommended to skip it, as it makes the integration faster (and reduces the memory footprint). 

Explanation of outputs
----------------------

- ``V`` Voltage trace of every compartment. A matrix of size (nsteps, n_comps)
- ``I_clamp`` also returned in the first argument, this is the clamping current when a compartment is being voltage clamped. This can be inter-leaved with the voltage of other, non-clamped compartments. 
- ``Ca`` Calcium concentration in every cell and the corresponding ``E_Ca`` (reversal potential of Calcium). A matrix of size (nsteps, n_comps)
- ``mech_state`` a matrix representing every dimension of every mechanism in the tree. This matrix has size (nsteps, NC), where NC depends on the precise controllers used, and is automatically determined. 
- ``I`` the currents of every ion channel type in the model. This is a matrix of size (nsteps, n_cond)



%}


function [V, Ca, cont_state, curr_state, syn_state] = integrate(self)


if isempty(self.linked_binary)
	if self.verbosity > 0
		disp(['[INFO] No linked binary, hashing...'])
	end
	h = self.hash;
	mexBridge_name = [joinPath(self.xolotl_folder,'X_') h '.cpp'];
	self.linked_binary = ['X_' h '.' self.OS_binary_ext];
end


% does the binary exist?
if exist(joinPath(self.xolotl_folder,self.linked_binary),'file') == 3
	% does the hash match up?

	if self.verbosity > 0
		disp(['[INFO] Binary exists.'])
	end

	h = self.hash;
	if ~strcmp(self.linked_binary(3:34),h)

		if self.verbosity > 0
			disp(['[INFO] Binary out of sync'])
			disp(['[INFO] Current hash is ' h])
		end

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

assert(rem(self.dt,self.sim_dt)==0,'Simulation & output dt are not compatible')

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
	% fill with zeros
	self.V_clamp = NaN(1,n_comp);
elseif  floor(self.t_end/self.sim_dt) ~= size(self.V_clamp,1) && size(self.V_clamp,1) > 1
	warning('Incompatible V_clamp, will be ignored.')
	self.V_clamp = NaN(n_steps,n_comp);
end

if isempty(self.I_ext)
	% fill with zeros
	self.I_ext = zeros(1,n_comp);
elseif  floor(self.t_end/self.sim_dt) ~= size(self.I_ext,1) && size(self.I_ext,1) > 1
	warning('Incompatible I_ext, will be ignored.')
	self.I_ext = zeros(n_steps,n_comp);
end

% vectorize the current state
arguments = self.serialize;


[~,f] = fileparts(self.linked_binary);

f = str2func(f);
[results{1:nargout+1}] = f(arguments,self.I_ext',self.V_clamp');

if self.closed_loop
	self.deserialize(results{1}(1:length(arguments)));
end

% read out mechanism sizes
mechanism_sizes = results{1}(length(arguments)+1:end);

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
