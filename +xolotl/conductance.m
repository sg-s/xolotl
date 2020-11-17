classdef conductance 

properties


	% activation functions
	m_inf function_handle
	h_inf function_handle
	tau_m function_handle
	tau_h function_handle

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
		customdir = [fileparts(fileparts(which('xolotl'))) filesep 'c++' filesep 'conductances' filesep 'custom'];
		if exist(customdir,'dir') ~= 7
			mkdir(customdir)
		end


		% check that everything is filled out 
		props = properties(self);
		for i = 1:length(props)
			corelib.assert(~isempty(self.(props{i})),['All properties must be filled out. ' props{i} ' is still not defined'])
		end

		% devectorize functions
		self.m_inf = str2func(strrep(func2str(self.m_inf),'./','/'));
		self.h_inf = str2func(strrep(func2str(self.h_inf),'./','/'));
		self.tau_m = str2func(strrep(func2str(self.tau_m),'./','/'));
		self.tau_h = str2func(strrep(func2str(self.tau_h),'./','/'));

		templatedir = [fileparts(fileparts(which('xolotl'))) filesep 'c++' filesep 'conductances' filesep 'templates' filesep];



		if self.is_Ca  
			lines = filelib.read([templatedir 'Ca_exact.hpp']);
		else
			lines = filelib.read([templatedir 'generic_exact.hpp']);
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
						error('Unrecognized function signature.')
					end

					if any(strfind(f,'NaN'))
						f = '  1';
					end


					% remove ^ and convert to pow
					if any(strfind(f,'^'))
						caretpos = strfind(f,'^');
						basepos = max(regexpi(f(1:caretpos-1),'\W'))+1:caretpos-1;
						temp = regexp(f(caretpos+1:end),'[^0-9.]')-1;
						exponentpos =  (caretpos+1:caretpos+temp(1));
						f = strrep(f,f(basepos(1):exponentpos(end)),['pow(',f(basepos),',',f(exponentpos),')']);
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


		filelib.write([customdir filesep name '.hpp'],lines);



	end % end generateCPPFile

	

end % end methods 



end % end classdef