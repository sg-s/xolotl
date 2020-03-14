classdef conductance 

properties


	% activation functions
	m_inf 
	h_inf 
	tau_m 
	tau_h 

	is_Ca (1,1) logical = false

	default_m (1,1) double = 0
	default_h (1,1) double = 1
	default_E (1,1) double

	p (1,1) double
	q (1,1) double




end


methods

	function generateCPPFile(self, name)

		corelib.assert(nargin == 2,'Not enough input arguments')


		% check that the "custom" folder exists
		if exist([fileparts(which(mfilename)) filesep 'c++' filesep 'conductances' filesep 'custom'],'dir') ~= 7
			mkdir([fileparts(which(mfilename)) filesep 'c++' filesep 'conductances' filesep 'custom'])
		end


		% check that everything is filled out 
		props = properties(self);
		for i = 1:length(props)
			corelib.assert(~isempty(self.(props{i})),['All properties must be filled out. ' props{i} ' is still not defined'])
		end

		this_dir = fileparts(which(mfilename));
		this_dir = [this_dir filesep 'c++' filesep 'conductances' filesep 'templates' filesep];



		if self.is_Ca  
			lines = filelib.read([this_dir 'Ca_exact.hpp']);
		else
			lines = filelib.read([this_dir 'generic_exact.hpp']);
		end


		for i = 1:length(props)
			idx = filelib.find(lines,['$' props{i}]);
			if any(idx)

				this_line = lines{idx};
				if isa(self.(props{i}),'function_handle')
						
					if any(strfind(char(self.(props{i})),'@(V)'))
						f = func2str(self.(props{i}));
						f = strrep(f,'@(V)',' ');

					elseif any(strfind(char(self.(props{i})),'@(V,Ca)'))
						f = func2str(self.(props{i}));
						f = strrep(f,'@(V,Ca)',' ');
					else
						error('Unrecognised function signature.')
					end

					if any(strfind(f,'NaN'))
						f = '  1';
					end


					this_line = strrep(this_line,['$' props{i}], f);
					
				else
					this_line = strrep(this_line,['$' props{i}], mat2str(self.(props{i})));
					
				end
				lines{idx} = this_line;

			end
		end

		% replace names as needed
		for i = 1:length(lines)
			lines{i} = strrep(lines{i},'UPPERCASENAME',upper(name));
			lines{i} = strrep(lines{i},'CondName',name);
		end


		filelib.write([fileparts(fileparts(this_dir)) filesep 'custom' filesep name '.hpp'],lines);



	end % end generateCPPFile

	

end % end methods 



end % end classdef