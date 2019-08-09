
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
% | `ibi_thresh` | 300 | ms |
% | `min_firing_rate` | 1 | Hz |
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

function [I_ext, ii, metrics] = rheobase(x, varargin)

  % options and defaults
  options = struct;
  options.current_steps = 0:0.01:1;
  options.debug = false;
  options.ibi_thresh = 300;
  options.min_firing_rate = 1;
  options.sampling_rate = 20;
  options.spike_threshold = 0;
  options.verbosity = true;

  options = orderfields(options);

  if nargout && ~nargin
    varargout{1} = options;
    return
  end

  % validate and accept options
  options = corelib.parseNameValueArguments(options, varargin{:});

  % save the initial state
  corelib.verb(options.verbosity && any(strcmp({x.snapshots.name}, 'rheobase')), 'rheobase', 'overwriting ''rheobase'' snapshot');
  x.snapshot('rheobase');

  for ii = 1:length(options.current_steps)
    corelib.verb(options.verbosity, 'rheobase', ['I_ext = ' num2str(options.current_steps(ii)) ' nA'])

    x.reset('rheobase');
    x.I_ext = options.current_steps(ii);

    x.integrate;
    V = x.integrate;

    metrics = xtools.V2metrics(V, options);

    if metrics.firing_rate >= options.min_firing_rate
      break
    end
  end % for loop

  corelib.verb(ii == length(options.current_steps) & options.verbosity, 'rheobase', ['maximum iterations reached'])

  I_ext = options.current_steps(ii);

end % function
