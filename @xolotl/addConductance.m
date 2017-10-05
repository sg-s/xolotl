function addConductance(self,compartment,cond_id,gbar,E,m,h)
	assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

	% search for cond_id
	cond_file = [];
	for i = 1:length(self.available_conductances)
		if any(strfind(self.available_conductances{i},cond_id))
			cond_file = i;
			break;
		end
	end
	assert(~isempty(cond_file),'Which conductance do you mean?')

	if nargin < 6
		m = 0;
		h = 1;
	end
	if nargin < 7
		h = 1;
	end

	cond_name = pathEnd(self.available_conductances{cond_file});

	self.(compartment).(cond_name).full_path = self.available_conductances{cond_file};
	self.(compartment).(cond_name).gbar = gbar;
	self.(compartment).(cond_name).E = E;
	self.(compartment).(cond_name).m = m;
	self.(compartment).(cond_name).h = h;

	% add this to conductance_headers, if it's not already there
	self.conductance_headers = [self.conductance_headers; self.available_conductances{cond_file}];
end