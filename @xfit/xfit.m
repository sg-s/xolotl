
% xfit is a toolbox that helps you find neuron or network
% models satisfying arbitrary constraints. It is a bridge
% between the Global Optimization Toolbox in MATLAB
% and the xolotl neuron and network simulator

classdef xfit < handle

properties
	x (1,1) xolotl

	% function to minimize
	SimFcn function_handle 

	% specify a function to plot results
	ShowFcn function_handle 

	% specify a function to initialize the model
	InitFcn function_handle

	% parameters to optimize
	FitParameters cell
	seed double
	lb double
	ub double

	SaveParameters cell
	SaveWhenCostBelow (1,1) double = Inf

	engine char 


	% this can be used to store any user-defined data
	data


	options 

	% nonlinear constrains, only supported
	% for some engines
	nonlcon function_handle

end % end props


properties (SetAccess = private)

	% logging
	best_cost
	timestamp

	SimFcnHash
end


methods
	function self = xfit(engine)
		% check for optimisation toolbox
		v = ver;
		gcp;
		if nargin == 0
			engine = 'particleswarm';
		end
		corelib.assert(any(strcmp('Optimization Toolbox', {v.Name})),'optimisation toolbox is required')
		corelib.assert(any(strcmp('Global Optimization Toolbox', {v.Name})),'Global Optimization Toolbox is required')
		self.engine = engine;

	end % end constructor

	function self = set.x(self,value)
		value.closed_loop = false;
		corelib.assert(length(value) == 1,'Only one Xolotl object at a time')
		self.x = value;
	end


	function self = set.SimFcn(self,value)

		% check that it exists

		if isempty(which(func2str(value)))
			error('SimFcn could not be found')
		end

		self.SimFcnHash = hashlib.md5hash(which(func2str(value)),'file');

		self.SimFcn = value;


	end


	function self = set.FitParameters(self,names)
		% check that a xolotl object is configured
		corelib.assert(~isempty(self.x),'First configure a xolotl object')

		% check that they all resolve correctly
		for i = 1:length(names)
			try
				self.x.get(names{i});
			catch

				error(['This name does not resolve to anything in the xolotl object tree: ' names{i}])
			end

		end
		self.FitParameters = names;
	end



	function self = set.engine(self,value)
		pool = gcp;
		switch value
		case 'patternsearch'
			self.engine = 'patternsearch';
			self.options = optimoptions('patternsearch');
			self.options.UseParallel = true;
			self.options.UseCompleteSearch = true;
			self.options.UseCompletePoll = true;
			self.options.Display = 'iter';
			self.options.MaxTime = 100;
			self.options.OutputFcn = @self.pattern_logger;
		case 'particleswarm'
			self.engine = 'particleswarm';
			self.options = optimoptions('particleswarm');
			self.options.UseParallel = true;
			self.options.ObjectiveLimit = 0;
			self.options.Display = 'iter';
			self.options.MaxTime = 100;
			self.options.OutputFcn = @self.swarm_logger;
			self.options.SwarmSize = 2*pool.NumWorkers;
		case 'ga'
			self.engine = 'ga';
			self.options = optimoptions('ga');
			self.options.UseParallel = true;
			self.options.FitnessLimit = 0;
			self.options.Display = 'iter';
			self.options.MaxTime = 100;
			self.options.OutputFcn = @self.ga_logger;
		otherwise
			error('Unknown engine')
		end
	end % set engine

	function stop = swarm_logger(self,optimValues,~)
		stop = false;

		self.best_cost(optimValues.iteration+1) = optimValues.bestfval;
		self.timestamp(optimValues.iteration+1) = now;

	end

	function [stop, options, optchanged] = pattern_logger(self,optimValues,options,~)
		stop = false;
		optchanged = false;
		self.best_cost(optimValues.iteration+1) = optimValues.fval;
		self.timestamp(optimValues.iteration+1) = now;
	end

	function [state, options, optchanged] = ga_logger(self, options, state, flag)
		optchanged = false;
		self.best_cost(state.Generation + 1) = min(state.Score);
		self.timestamp(state.Generation + 1) = now;
	end % end ga logger

end % end methods




end % end classdef
