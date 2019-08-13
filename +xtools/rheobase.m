
%
% ;;     ;; ;;;;;;;;  ;;;;;;;   ;;;;;;;  ;;        ;;;;;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;       ;;
%    ;;;       ;;    ;;     ;; ;;     ;; ;;        ;;;;;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;             ;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
% ;;     ;;    ;;     ;;;;;;;   ;;;;;;;  ;;;;;;;;  ;;;;;;
%
% ### rheobase
%
%
% **Syntax**
%
% ```matlab
% [I_ext, index, metrics] = xtools.rheobase(x)
% [I_ext, index, metrics] = xtools.rheobase(x, 'PropertyName', PropertyValue, ...)
% [I_ext, index, metrics] = xtools.rheobase(x, options)
% ```
%
% **Description**
%
% Finds the minimum injected current required to cause a xolotl model to spike.
% The model is simulated with constant injected current determined by the `current_steps` option,
% which is a vector of real numbers,
% until the minimum firing rate is reached.
% The output `I_ext` contains the current magnitude needed to cause the model to spike,
% `index` contains the linear index into the `current_steps` vector.
% `metrics` contains a struct of computed statistics for the resultant spike train.
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
% | `current_steps` | 0:0.01:1 | nA |
% | `debug` | false | |
% | `MinFiringRate` | 1 | Hz |
% | `sampling_rate` | 20 | 1/ms |
% | `spike_threshold` | 0 | mV |
% | `verbosity` | true | |
%
%
%
% !!! info "See Also"
%     xtools.V2metrics
%     xtools.findNSpikes
%     xtools.findNSpikeTimes
%

function I = rheobase(x, varargin)

% options and defaults
options.MinI = -2;
options.MaxI = 4;
options.SpikeThreshold = 0;
options.t_end = 10e3;


% validate and accept options
options = corelib.parseNameValueArguments(options, varargin{:});

% save the initial state
x.reset;
x.snapshot('rheobase');


I = fminbnd(@(y) nSpikesForCurrent(x,y) ,options.MinI,options.MaxI);



function N = nSpikesForCurrent(x, I_ext)

	x.reset('rheobase');
	x.I_ext = I_ext;

	x.closed_loop = true;
	x.integrate;
	V = x.integrate;

	N = xtools.findNSpikes(V(:,1));
	if N == 0
		N = abs(I_ext);
	end

end


end