classdef xolotl < handle

properties (SetAccess = protected)

	compartment_props 

end  % end props

methods 
	function self = xolotl()
		% read props from compartment.h
		cppfilename = joinPath(fileparts(which(mfilename)),'compartment.h');
		self.compartment_props = findCPPClassMembers(cppfilename);
	end

	function compartment()
	end


end % end methods 


end % end classdef 