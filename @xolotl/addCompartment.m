function addCompartment(self,label, varargin)
	assert(nargin > 1,'No label! You have to label every compartment')

	% check that there are as many inputs as there are compartment_props
	assert(length(self.compartment_props) ==  length(varargin),'Wrong number of compartment properties' )

	self.addprop(label);

	% blindly trust that the arguments make sense and are in the correct order
	for i = 1:length(varargin)
		self.(label).(self.compartment_props{i}) = varargin{i};
	end

	self.compartment_names = [self.compartment_names; label];
end