
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### integrate
%
% integrates a `xolotl` model.
%
% **Syntax**
%
% ```matlab
% x.output_type = 0;
% V = x.integrate;
% I_clamp = x.integrate;
% [V, Ca] = x.integrate;
% [V, Ca, mech_state] = x.integrate;
% [V, Ca, mech_state, I] = x.integrate;
% [V, Ca, mech_state, I, syn_state] = x.integrate;
%
% x.output_type = 1;
% results = x.integrate;
%
% x.output_type = 2;
% results_and_spiketimes = x.integrate;
% ```
%
% **Description**
%
% The outputs of integrate depend on the `output_type` property of `xolotl`.
%
% | `output_type` value | outputs of `x.integrate` |
% | ------------------- | ------------------------ |
% | 0 (default) | up to 5 matrices of type double |
% | 1 | only one output, a structure |
% | 2 | only one output, a structure |
%
%
% **Explanation of outputs**
%
% When `output_type` is 0,
%
% - `V` Voltage trace of every compartment. A matrix of size (nsteps, n_comps)
% - `I_clamp` also returned in the first argument, this is the clamping current when a compartment is being voltage clamped. This can be inter-leaved with the voltage of other, non-clamped compartments.
% - `Ca` Calcium concentration in every cell and the corresponding `E_Ca` (reversal potential of Calcium). A matrix of size (nsteps, n_comps)
% - `mech_state` a matrix representing every dimension of every mechanism in the tree. This matrix has size (nsteps, NC), where NC depends on the precise controllers used, and is automatically determined.
% - `I` the currents of every ion channel type in the model. This is a matrix of size (nsteps, n_cond)
%
% When `output_type` is 1 or 2, the integration is performed requesting all outputs, and these outputs are organized in a structure and named to match the names of the components in the model.
%
%
% See Also: 
% xolotl.show
% xolotl.plot
% xolotl.transpile
% xolotl.compile
 




function varargout = integrate(self)


if isempty(self.linked_binary)
	if rem(self.verbosity,2) == 0
		disp('[INFO] No linked binary, hashing...')
	end
	h = self.hash;
	self.linked_binary = ['X_' h '.' mexext];
else
	if rem(self.verbosity,2) == 0
		disp(['[INFO] Linked binary is:' self.linked_binary])
	end
end


% does the binary exist?
if exist(fullfile(filelib.cachePath('xolotl'),self.linked_binary),'file') > 0
	% does the hash match up?

	if rem(self.verbosity,2) == 0
		disp('[INFO] Binary exists.')
	end

	h = self.hash;
	if ~strcmp(self.linked_binary(3:34),h)

		if rem(self.verbosity,2) == 0
			disp('[INFO] Binary out of sync')
			disp(['[INFO] Current hash is ' h])
		end

		% maybe the binary exists?
		if exist(fullfile(filelib.cachePath('xolotl'),['X_' h '.' mexext]),'file') > 0
			% binary exists. just update the linked_binary and we should be good
			if rem(self.verbosity,2) == 0
				disp('[INFO] Binary exists, no need to recompile.')
			end
			self.linked_binary = ['X_' h '.' mexext];
		else
			% no, need to compile

			self.transpile;
			self.compile;
		end
	end

else
	self.transpile;
	self.compile;
end

% check that sim_dt and output_dt make sense
if isnan(self.sim_dt) || isempty(self.sim_dt)
	self.sim_dt = self.dt;
end

corelib.assert(self.t_end > 1,'t_end too short')
corelib.assert(self.sim_dt > 0,'sim_dt must be positive')
corelib.assert(self.dt > 0,'dt must be positive')

% check that the simulation doesn't exceed C++ intmax
corelib.assert(self.t_end/self.sim_dt<intmax,'Simulation is too long. The number of steps required exceeds INT_MAX')

if self.dt < self.sim_dt
	self.dt = self.sim_dt;
end

corelib.assert(rem(self.dt,self.sim_dt)==0,'Simulation & output dt are not compatible')

if nargout == 0 & self.closed_loop == false
	error('Are you sure you want to integrate this with no outputs and with closed_loop set to FALSE?')
end


if self.output_type == 0
	n_outputs = nargout;
elseif self.output_type == 1
	n_outputs = 5; % we get everything
elseif self.output_type == 2
	n_outputs = 6; % everything + spiketimes
else
	error('output_type should be either 0, 1 or 2')
end

V = [];
Ca = [];
I_clamp = [];
curr_state = [];
syn_state = [];
mech_state = [];
spiketimes = [];

comp_names = self.find('compartment');
n_comp = length(comp_names);
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
args = self.serialize;

[~,f] = fileparts(self.linked_binary);
f = str2func(f);


% now do the actual integration

if self.pref.cache
	hash = hashlib.md5hash([self.hash hashlib.md5hash(self.serialize) hashlib.md5hash(nargout)]);
	cachename =  fullfile(userpath,'xolotl',[hash '.cache']);
	if exist(cachename,'file') == 2
		% cache exists, load that and don't do anything
		disp('Using cache...')
		load(cachename,'results','-mat')
	else
		[results{1:n_outputs+1}] = f(args,self.I_ext',self.V_clamp');
	end

else
	[results{1:n_outputs+1}] = f(args,self.I_ext',self.V_clamp');
end




if self.closed_loop
	self.deserialize(results{1}(1:length(args)));
end



if n_outputs > 0
	V = (results{2})';
	varargout{1} = V;
end
if n_outputs > 1
	Ca = (results{3})';
	varargout{2} = Ca;
end
if n_outputs > 2
	mech_state = (results{4})';
	varargout{3} = mech_state;
end
if n_outputs > 3
	curr_state = (results{5})';
	varargout{4} = curr_state;
end
if n_outputs > 4
	syn_state = (results{6})';
	varargout{5} = syn_state;
end


if self.pref.cache
	save(cachename,'results')
end


if self.output_type == 0
	return
end



% return a data structure


data = self.structureOutputs(V,Ca,mech_state,curr_state,syn_state);

% voltages/spikes
if self.output_type == 2
	spiketimes = results{7};
	% spiketimes only
	for i = 1:n_comp
		data.(comp_names{i}).spiketimes = nonzeros(spiketimes(:,i));
	end
end

varargout{1} = data;