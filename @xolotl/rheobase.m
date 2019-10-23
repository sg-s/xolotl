

%
% ### rheobase
%
%
% **Syntax**
%
% ```matlab
% options = x.rheobase
% I = x.rheobase('PropertyName', PropertyValue, ...)
% I = x.rheobase(options)
% ```
%
% **Description**
%
% Finds the minimum injected current required to cause a xolotl model to spike.
% The model is simulated with increasing amounts of constant injected current until one spike is elicited.
% This minimum amount of current is called the rheobase.
% The output `I` contains the current magnitude needed to cause the model to spike,
%
% If called without arguments and one output, a struct
% containing fields for all optional arguments, `options`,
% is returned.
%
% Otherwise, the first argument should be a xolotl object,
% and the latter should be either name, value keyword pairs,
% or a struct to specify options.
% Options with a `NaN` value are ignored, and the default is used instead.
%
% | Option Name | Default Value | Units |
% | ----------- | ------------- | ----- |
% | `I_min` | -0.2 | nA |
% | `I_max` | 4 | nA |
% | `SpikeThreshold` | 0 | mV |
% | `nSpikes` | 1 | |
% | `t_end` | 10e3 | ms |
%
% `I_min` and `I_max` define the minimum and maximum of the tested injected current.
% The `SpikeThreshold` defines when a spike is said to occur.
% The voltage must cross this threshold in order for a spike to be counted.
% You can change the number of spikes to search for via the `nSpikes` option.
% This should be used in combination with the `t_end` option,
% which defines how long the simulation is.
% For example, to find the current at which your model spikes at 10 Hz,
% you could set `t_end = 10e3` and `nSpikes = 10`.
%
%
% !!! info "See Also"
%     xtools.V2metrics
%     xtools.findNSpikes
%     xtools.findNSpikeTimes
%     xolotl.fI
%

function I = rheobase(self, varargin)

% options and defaults
options.I_min = -2;
options.I_max = 4;
options.SpikeThreshold = 0;
options.nSpikes = 1;
options.t_end = 10e3;


% validate and accept options
options = corelib.parseNameValueArguments(options, varargin{:});

% if one output and no arguments, output the `options` structure
if nargout && ~nargin
	varargout{1} = options;
	return
end

% save the initial state
self.reset;
self.snapshot('rheobase');


I = fminbnd(@(y) nSpikesForCurrent(self, y, options.nSpikes) ,options.I_min,options.I_max);



function N = nSpikesForCurrent(x, I_ext, nSpikes)

	x.reset('rheobase');
	x.I_ext = I_ext;

	x.closed_loop = true;
	x.integrate;
	V = x.integrate;

	if any(isnan(V))
		N = 1e9;
		return
	end

	N = xtools.findNSpikes(V(:,1)) - nSpikes;
	if N < 0
		N = abs(I_ext);
	end

end


end
