% xolotl.m
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
	linked_binary@char
	synapses
    synapse_pre
    synapse_post
end  % end set protected props

properties (Access = protected)
	xolotl_folder
	cpp_folder
	OS_binary_ext % OS-specific
	dyn_prop_handles % handles to dynamic properties 
	illegal_names = {'xolotl_network','compartment','conductance','controller','synapse','network','x','self'}; % list of illegal names for compartments, synpases and other objects
	

	% this stores custom owners of some objects
	% specified by put
	custom_owner

    snapshots

	% this stores information about calling methods
	% of some objects after they have been created 
	call_method_data

    hpp_files

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
	closed_loop@logical = true;
	temperature@double = 11; % centigrade 
	temperature_ref@double = 11; % centigrade 

    manipulate_plot_func@cell;
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
        if ~isempty(self.call_method_data)
        	for i = 1:length(self.call_method_data)
                disp([self.call_method_data(i).object, '.' , self.call_method_data(i).method_name ,'(', self.call_method_data(i).method_values , ')'])
            end
        end


    end


end % end protected methods

methods 
	function self = xolotl()
		self.rebase;

        % append all classnames to illegal names
        [~,hpp_files] = self.resolvePath('');
        self.hpp_files = hpp_files;
        for i = 1:length(hpp_files)
            try
                hpp_files{i} = pathEnd(hpp_files{i});
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


    function self = set.V_clamp(self,V_clamp)
        n_comp = length(self.find('compartment'));
        n_steps = floor(self.t_end/self.sim_dt);

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

        n_comp = length(self.find('compartment'));
        n_steps = floor(self.t_end/self.sim_dt);

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


    function search(self,str)
        idx = lineFind(self.hpp_files,str);
        disp(unique(self.hpp_files(idx)))
    end


end % end methods 

methods (Static)

    cleanup;
    curr_index = contributingCurrents(V,I);
    C = coincidence(model_spiketimes, data_spiketimes, dt, Delta);
    spiketimes = findNSpikeTimes(V,n_spikes,on_off_thresh);
    f = findNSpikes(V, on_off_thresh);
    ax = show(conductance,ax);
    [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance);


end % end static methods
end % end classdef 