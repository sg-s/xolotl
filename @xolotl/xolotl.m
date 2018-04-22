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
end  % end set protected props

properties (Access = protected)
	xolotl_folder
	cpp_folder
	OS_binary_ext % OS-specific
	dyn_prop_handles % handles to dynamic properties 
	illegal_names = {'xolotl_network','compartment','conductance','controller','synapse','network','x','self'}; % list of illegal names for compartments, synpases and other objects
	synapse_pre
	synapse_post

	% this stores custom owners of some objects
	% specified by put
	custom_owner

	% this stores information about calling methods
	% of some objects after they have been created 
	call_method_data

    hpp_files

end  % end protected props


properties
	debug_mode@logical = false
	controllers@cell = {}

	% output delta t
	dt@double = 50e-3; % ms

	% simulation deltat t
	sim_dt@double = NaN;

	t_end@double = 5000; % ms
	handles
	closed_loop@logical = true;
	temperature@double = 11; % centigrade 
	temperature_ref@double = 11; % centigrade 
end % end general props


methods (Access = protected)
    function displayScalarObject(self)
        url = 'https://github.com/sg-s/xolotl/';
        fprintf(['\b\b\b\b\b\b\b\b\b<a href="' url '">xolotl</a> object with:\n'])
        fprintf('---------------------\n')
        compartment_names = self.find('compartment');
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
	end


    function search(self,str)
        idx = lineFind(self.hpp_files,str);
        disp(unique(self.hpp_files(idx)))
    end


end % end methods 

methods (Static)

    ax = plot(conductance,ax);
    [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance);


end % end static methods
end % end classdef 