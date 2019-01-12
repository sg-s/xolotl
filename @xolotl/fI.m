%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### fI

**Syntax**

```matlab
data = x.fI()
data = x.fI('Name',value...)
```

**Description**

This method computes the f-I (firing-rate vs current) 
curve of a single compartment model. `data` is a structure containing the following fields:

* `I` vector of injected currents
* `f_up` firing rates when going up the curve
* `f_down` firing rates when going down the curve
* `CV_ISI_up` coefficient of variation of inter-spike intervals when going up the curve 
* `CV_ISI_down` coefficient of variation of inter-spike intervals when going down the curve 

The following optional parameters may be specified in name-value syntax:

| Name | Allowed Values | Default |
| ----- | ----------- | ---------- |
| I_min | any scalar | - .1 |
| I_max | any scalar | 1 | 
| n_steps | +ve integer | 10 |
| t_end | +ve integers | 1e4 | 

%}

function data = fI(self, varargin)



% options and defaults
options.I_min = -.1;
options.I_max = 1;
options.n_steps = 10;
options.t_end = 10e3;

if nargout && ~nargin 
	varargout{1} = options;
    return
end

% validate and accept options
if iseven(length(varargin))
	for ii = 1:2:length(varargin)-1
	temp = varargin{ii};
    if ischar(temp)
    	if ~any(find(strcmp(temp,fieldnames(options))))
    		disp(['Unknown option: ' temp])
    		disp('The allowed options are:')
    		disp(fieldnames(options))
    		error('UNKNOWN OPTION')
    	else
    		options.(temp) = varargin{ii+1};
    	end
    end
end
elseif isstruct(varargin{1})
	% should be OK...
	options = varargin{1};
else
	error('Inputs need to be name value pairs')
end



data = struct;

I0 = self.I_ext;
S = serialize(self);

assert(options.I_min < options.I_max,'I_min must be less than I_max')
assert(options.n_steps > 2, 'n_steps too small::At least two points needed')

assert(length(self.find('compartment')) == 1,' You can only compute the f-I curve for single compartment models')

assert(isint(options.n_steps),'n_steps must be an integer')



all_I_ext = [linspace(options.I_min,options.I_max,options.n_steps) linspace(options.I_max,options.I_min,options.n_steps)];


data.I = linspace(options.I_min,options.I_max,options.n_steps);
data.f_up = NaN*linspace(options.I_min,options.I_max,options.n_steps);
data.f_down = NaN*linspace(options.I_min,options.I_max,options.n_steps);
data.CV_ISI_up = linspace(options.I_min,options.I_max,options.n_steps);
data.CV_ISI_down = linspace(options.I_min,options.I_max,options.n_steps);



I_ext = (repmat(all_I_ext,options.t_end/self.sim_dt,1));
I_ext = I_ext(:);
self.t_end = options.t_end*length(all_I_ext);

self.I_ext = I_ext;

original_approx_state = self.approx_channels;
self.approx_channels = 0;

V = self.integrate;
V = (reshape(V,length(V)/(length(all_I_ext)),length(all_I_ext)));

self.approx_channels = original_approx_state;

idx = 0;

for i = 1:length(all_I_ext)
	metrics = xtools.V2metrics(V(:,i),'sampling_rate',1./self.dt);

	if i <= length(all_I_ext)/2
		idx = idx + 1;
		data.f_up(idx) = metrics.firing_rate;
		data.CV_ISI_up(idx) = metrics.isi_std/metrics.isi_mean;
	else

		data.f_down(idx) = metrics.firing_rate;
		data.CV_ISI_down(idx) = metrics.isi_std/metrics.isi_mean;
		idx = idx - 1;
	end



end


% restore the xolotl object the its original state
self.deserialize(S);
self.I_ext = I0;
