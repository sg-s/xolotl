
classdef xfind < handle



properties (Access = private)

end % private props

properties

	x (1,1) xolotl

	% names of parameters
	ParameterNames (:,1) cell

	% bounds
	Upper (:,1) double
	Lower (:,1) double 


	% stores futures of executing parallel tasks
	workers (:,1) parallel.FevalFuture	

	% allow user-defined custom sample function
	SampleFcn function_handle

	% allow user to run some function on the model
	% after we sample it
	% should return a structure data with data
	SimFcn function_handle 

	% this function determines if the sampled point
	% is logged or not. 
	% data is the data returned by SimFcn
	DiscardFcn (1,1) function_handle = @(data) false

end % props



methods 

	function self = xfind()
		self.SampleFcn = @self.uniformRandom;
		self.SimFcn = @self.measureFiringRate;
	end % constructor



	% helper method that writes to disk
	function writeToDisk(self,f, data)
		params = self.x.get(self.ParameterNames);

		% stream data to disk
		fwrite(f,params,'double');
		fwrite(f,data,'double');

	end


	% returns a hash of the neuron model
	% that is used to save data
	function H = hash(self)
		H = self.x.hash(1:5);
	end


	% picks a point from a uniform random distribution
	% within bounds
	function uniformRandom(self)
		params = (self.Upper-self.Lower).*rand(length(self.Upper),1) + self.Lower;
		self.x.set(self.ParameterNames,params);
	end % uniformRandom


	% this example function simulates the model and
	% measures the firing rate in the first compartment
	function data = measureFiringRate(self)
		V = self.x.integrate;
		data = xtools.findNSpikes(V(:,1))/(self.x.t_end*1e-3);
	end





end % methods



methods (Static)



end % static methods






end % classdef

