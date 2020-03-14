%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% a MATLAB class that wraps C++ code
% that runs multi-compartment neuron/networks
% it generates C++ files, compiles them, and runs them
% based on pseudo-objects that you can define within it
%
% Srinivas Gorur-Shandilya
% see https://github.com/sg-s/xolotl
% for more information


classdef xolotl <  cpplab & matlab.mixin.CustomDisplay & ConstructableHandle & UpdateableHandle

properties (SetAccess = protected)
	linked_binary char % binary to run when integrate is called
	synapses struct % structure containing synapses in model
    illegal_names cell = {'xolotl_network','compartment','conductance','controller','synapse','network','x','self'}; % list of illegal names for compartments, synpases and other objects

    snapshots struct % saves snapshots of models 

end  % end set protected props

properties (Access = protected, Transient = true)
	xolotl_folder % full path to folder that contains xolotl code
	cpp_folder % full path to folder that contains c++ code


end  % end protected props


properties
	verbosity (1,1) double {mustBeFinite(verbosity)} = 0;

    I_ext double;
    V_clamp double;

	% output delta t
	dt (1,1) double {mustBeGreaterThan(dt,0)} = .1; % ms

	% simulation deltat
	sim_dt (1,1) double {mustBeGreaterThan(sim_dt,0)} = .1;
	t_end (1,1) double {mustBeGreaterThan(t_end,1), mustBeInteger(t_end)} = 5000; % ms

	handles 
	closed_loop (1,1) logical = true;
	temperature (1,1) double {mustBeGreaterThan(temperature,0), mustBeLessThan(temperature,50)} = 11; % centigrade
	temperature_ref (1,1) double {mustBeGreaterThan(temperature_ref,0), mustBeLessThan(temperature_ref,50)} = 11; % centigrade

    manipulate_plot_func cell

    solver_order (1,1) double = 0;

    % should we use the current to integrate voltage
    % in single compartments? 
    use_current (1,1) double {mustBeInteger(use_current), mustBeLessThan(use_current, 2)} = 0;

    % should we approximate gating functions?
    % 0 -- no approximations
    % 1 -- integer mV only (approx)
    approx_channels (1,1) double {mustBeInteger(approx_channels), mustBeLessThan(approx_channels, 2)} = 1;

    % should we integrate channels deterministically?
    % 0 -- deterministic
    % 1 -- Langevin approximation
    stochastic_channels (1,1) double {mustBeInteger(stochastic_channels), mustBeLessThan(stochastic_channels, 2)} = 0; 

    % structure that stores preferences
    % edit pref.m to change these
    pref struct


    % what sort of output do you desire?
    % 0 -- standard, V, Ca, etc. separated into variables
    % 1 -- a structure. all outputs included
    % 2 -- structure, but only with spike times
    output_type (1,1) double {mustBeInteger(output_type), mustBeLessThan(output_type, 3)} = 0
    spike_thresh (1,1) double {mustBeFinite(spike_thresh), mustBeReal(spike_thresh)} = 0 % mV


end % end general props


methods (Access = protected)
    function displayScalarObject(self)
        url = 'https://github.com/sg-s/xolotl/';
        fprintf(['\b\b\b\b\b\b\b\b\b<a href="' url '">xolotl</a> object with '])

        compartment_names = self.find('compartment');

        if length(compartment_names) > 20
            fprintf([mat2str(length(compartment_names)) ' compartments\n'])
            return
        else
            fprintf('\n---------------------\n')
        end

        for i = 1:length(compartment_names)
        	compartment = compartment_names{i};
        	url = ['matlab:' inputname(1) '.' compartment];
        	fprintf(['+ <a href="' url '">' compartment_names{i} '</a>  \n'])
        	% now show the conductances within this channel
        	C = self.(compartment).find('conductance');
        	for j = 1:length(C)
        		url = ['matlab:' inputname(1) '.' compartment '.' C{j}];
        		url_str = ['<a href="' url '">' C{j} '</a>'];
        		if isa(self.(compartment).(C{j}).gbar,'function_handle')
        			g = strrep(func2str(self.(compartment).(C{j}).gbar),'@()','');
        		else
        			g = strlib.oval(self.(compartment).(C{j}).gbar);
        		end
        		if isa(self.(compartment).(C{j}).E,'function_handle')
        			E = strrep(func2str(self.(compartment).(C{j}).E),'@()','');
        		else
        			E = strlib.oval(self.(compartment).(C{j}).E);
        		end
        		info_str = [' (g=' g ', E=' E ')'];



        		fprintf(['  > ' url_str info_str '\n'])
        	end
        	fprintf('---------------------\n')
        end


    end


end % end protected methods

methods
	function self = xolotl(varargin)
        self = self@ConstructableHandle(varargin{:});   
		self.rebase;

        self.pref = corelib.readPref(which(mfilename));

        % append all classnames to illegal names
        [~,hpp_files] = self.resolvePath('');

        for i = 1:length(hpp_files)
            try
                [~,hpp_files{i}] = fileparts(hpp_files{i});
            catch
                hpp_files{i} = '';
            end
        end
        hpp_files = unique(hpp_files);
        self.illegal_names = [self.illegal_names(:); hpp_files];

        self.setHiddenProps({'manipulate_plot_func','I_ext','V_clamp','snapshots'});

        self.snapshots = struct('name','','V',[],'hash','');

        % also configure the real names of props
        % for the better-serialize branch of cpplab
        if ~isfield(self,'cpp_lab_real_names_hash')
            self.cpp_lab_real_names = {'approx_channels';'dt' ;'output_type';'sim_dt';'solver_order';'spike_thresh'   ;'stochastic_channels';'t_end';'temperature';'temperature_ref';'use_current';'verbosity'};
        end
	end



    function self = set.V_clamp(self,V_clamp)

        d = dbstack;
        if strcmp(d(end).name,'copy')
            self.V_clamp = V_clamp;
            return
        end

        n_comp = length(self.find('compartment'));
        n_steps = floor(self.t_end/self.sim_dt);

        if n_comp == 0
            % probably loading from a file
            self.V_clamp = V_clamp;
            return
        end

        % make sure that it's the right size
        corelib.assert(size(V_clamp,2) == n_comp,'Size of V_clamp is incorrect::2nd dimension size should be n_comp')
        if size(V_clamp,1) ~= 1
            corelib.assert(size(V_clamp,1) == n_steps,'Size of V_clamp is incorrect::1st dimension size should be n_steps')
        end

        d = dbstack;

        if any(strcmp({d.name},'xolotl.set.I_ext'))
            % this is being called by setting I_ext, so
            % do nothing
        else
            % ignore I_ext, since it's being clamped
            self.I_ext = zeros(1,n_comp);
        end



        self.V_clamp = V_clamp;
    end

     function self = set.I_ext(self,I_ext)

        d = dbstack;
        if strcmp(d(end).name,'copy')
            self.I_ext = I_ext;
            return
        end



        n_comp = length(self.find('compartment'));
        n_steps = floor(self.t_end/self.sim_dt);

        if n_comp == 0
            % probably loading from a file
            self.I_ext = I_ext;
            return
        end

        if isvector(I_ext) && length(I_ext) == n_comp
            I_ext = I_ext(:)';
        end

        % make sure that it's the right size
        corelib.assert(size(I_ext,2) == n_comp,['Size of I_ext is incorrect::2nd dimension size should be ' mat2str(n_comp)])
        if size(I_ext,1) ~= 1
            corelib.assert(size(I_ext,1) == n_steps,['Size of I_ext is incorrect::1st dimension size should be ' mat2str(n_steps)])
        end

        d = dbstack;
        self.I_ext = I_ext;
        if any(strcmp({d.name},'xolotl.set.V_clamp'))

            % it's being called by V_clamp, so do nothing
        else
            % this is being called by the user, so we need to
            % cancel out V_clamp
            self.V_clamp = NaN(1,n_comp);
        end
    end



end % end methods

methods (Static)

    go_to_examples();
    
    uninstall();
    update();
    
    [passed, total] = run_all_tests(cleanup);
    cleanup;
    
    curr_index = contributingCurrents(V,I);    
    [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance)
    setup();

    b = loadobj(a);

end % end static methods
end % end classdef
