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

classdef xolotl <  cpplab & matlab.mixin.CustomDisplay

properties (SetAccess = protected)
	linked_binary@char % binary to run when integrate is called
	synapses@struct % structure containing synapses in model
    illegal_names = {'xolotl_network','compartment','conductance','controller','synapse','network','x','self'}; % list of illegal names for compartments, synpases and other objects

    snapshots % saves snapshots of models 

end  % end set protected props

properties (Access = protected)
	xolotl_folder % full path to folder that contains xolotl code
	cpp_folder % full path to folder that contains c++ code


end  % end protected props


properties
	verbosity@double = 0;

    I_ext@double;
    V_clamp@double;

	% output delta t
	dt@double = 50e-3; % ms

	% simulation deltat
	sim_dt@double = 50e-3;
	t_end@double = 5000; % ms

	handles
	closed_loop = true;
	temperature@double = 11; % centigrade
	temperature_ref@double = 11; % centigrade

    manipulate_plot_func@cell

    solver_order@double = 0;

    % should we approximate gating functions?
    % 0 -- no approximations
    % 1 -- integer mV only (approx)
    approx_channels@double = 0;

    % structure that stores preferences
    % edit pref.m to change these
    pref


    % what sort of output do you desire?
    % 0 -- standard, V, Ca, etc. separated into variables
    % 1 -- a structure. all outputs included
    % 2 -- structure, but only with spike times
    output_type@double = 0
    spike_thresh@double = 0 % mV


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
        			g = oval(self.(compartment).(C{j}).gbar);
        		end
        		if isa(self.(compartment).(C{j}).E,'function_handle')
        			E = strrep(func2str(self.(compartment).(C{j}).E),'@()','');
        		else
        			E = oval(self.(compartment).(C{j}).E);
        		end
        		info_str = [' (g=' g ', E=' E ')'];



        		fprintf(['  > ' url_str info_str '\n'])
        	end
        	fprintf('---------------------\n')
        end


    end


end % end protected methods

methods
	function self = xolotl()
		self.rebase;

        self.pref = readPref(which(mfilename));

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

        self.snapshots = struct('name','','V',[]);

        self.manipulate_plot_func{1} = @self.plot;
	end


    function self = set.closed_loop(self,value)
        if isscalar(value)
            self.closed_loop = logical(value);
        else
            error('xolotl::closed_loop must be a logical scalar, either "True" or "False"')
        end
    end % set.closed_loop


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
        assert(size(V_clamp,2) == n_comp,'Size of V_clamp is incorrect::2nd dimension size should be n_comp')
        if size(V_clamp,1) ~= 1
            assert(size(V_clamp,1) == n_steps,'Size of V_clamp is incorrect::1st dimension size should be n_steps')
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
        assert(size(I_ext,2) == n_comp,['Size of I_ext is incorrect::2nd dimension size should be' mat2str(n_comp)])
        if size(I_ext,1) ~= 1
            assert(size(I_ext,1) == n_steps,['Size of I_ext is incorrect::1st dimension size should be ' mat2str(n_steps)])
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
    ax = show(conductance,ax);
    [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance);


    setup();

end % end static methods
end % end classdef
