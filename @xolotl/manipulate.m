%{ 
           _       _   _
__  _____ | | ___ | |_| |
\ \/ / _ \| |/ _ \| __| |
>  < (_) | | (_) | |_| |
/_/\_\___/|_|\___/ \__|_|

### manipulate



**Syntax**

```matlab
x.manipulate();
x.manipulate('some*pattern')
x.manipulate({'parameter1','parameter2'})
```

**Description**

`manipulate` is a method that allows you to manipulate some or all parameters in a model while visualizing its behavior. 

- **`x.manipulate()`** manipulates all the parameters in a xolotl model. It wires up sliders to all parameters, and moving these sliders causes the model to integrate, and a plot to update. 
- **`x.manipulate('some*pattern')`** creates sliders only for parameters specified by 'some*pattern'. 
- **`x.manipulate({'parameter1','parameter2'})`** creates sliders only for the parameters specified in the cell array. Parameters should resolve to valid properties of cpplab objects in the tree. 


!!! info "See Also"
    ->xolotl.plot

%}



function manipulate(self, manipulate_these)


if isempty(self.linked_binary)
	self.transpile;
	self.compile;
end

% disable closed loop
self.closed_loop = false;

% make a snapshot of this now
self.snapshot('manipulate_zero')


t_end = self.t_end;

compartment_names = self.find('compartment');
n = length(compartment_names);


% I-ext cannot be a matrix
if self.pref.use_I_ext
	if size(self.I_ext,1) > 1
		warning('Dynamic I_ext not allowed while manipulating model. Resetting I_ext...')
		self.I_ext = zeros(1,length(self.Children));
	end
end


if nargin < 2

	[values, ~, ~, real_names] = self.serialize;

	% skip some dynamical values
	rm_this = [lineFind(real_names,'*dt'); lineFind(real_names,'*.m'); lineFind(real_names,'*.h'); lineFind(real_names,'synapses*.s')];

	% manually remove all the V, Ca for each neuron
	for i = 1:length(real_names)
		for j = 1:n
			if strcmp(real_names{i}, [compartment_names{j} '.Ca'])
				rm_this = [rm_this; i];
			end
			if strcmp(real_names{i}, [compartment_names{j} '.V'])
				rm_this = [rm_this; i];
			end
		end
	end

	values(rm_this) = [];
	real_names(rm_this) = [];

	% remove some parameters of xolotl
	rm_this = [];
	rm_this = [rm_this; find(strcmp(real_names,'verbosity'))];
	rm_this = [rm_this; find(strcmp(real_names,'solver_order'))];
	rm_this = [rm_this; find(strcmp(real_names,'output_type'))];
	rm_this = [rm_this; find(strcmp(real_names,'approx_channels'))];
	rm_this = [rm_this; find(strcmp(real_names,'approx_channels'))];
	rm_this = [rm_this;  lineFind(real_names,'tree_idx')];
	
	values(rm_this) = [];
	real_names(rm_this) = [];

	% add in I_ext for every neuron
	if self.pref.use_I_ext
		I_ext_names = self.Children;
		for i = 1:length(I_ext_names)
			I_ext_names{i} = ['I_ext_', I_ext_names{i}];
		end
		values = [values; zeros(length(self.Children),1)];
		real_names = [real_names; I_ext_names(:)];
	end

else

	if ~iscell(manipulate_these) && any(strfind(manipulate_these,'*'))
		% first find objects, then get them
		manipulate_these = self.find(manipulate_these);
	end

	real_names = manipulate_these;
	values = NaN*ones(length(real_names),1);
	
	% check that these exist
	for i = 1:length(real_names)
		try
			temp =  self.get(real_names{i});
			if isa(temp,'function_handle')
				temp = temp();
			end
			values(i) = temp;
		catch
			error(['This parameter could not be resolved: ' real_names{i}])
		end
	end
end


% semi-intelligently make the upper and lower bounds
lb = values/3;
ub = values*3;

% create a puppeteer instance and configure
warning('off','MATLAB:hg:uicontrol:ValueMustBeInRange')
warning('off','MATLAB:hg:uicontrol:MinMustBeLessThanMax')
p = puppeteer(real_names,values,lb,ub,[]);
self.handles.puppeteer_object = p;



for i = length(self.manipulate_plot_func):-1:1
	self.reset('manipulate_zero')
	self.manipulate_plot_func{i}(self);
end

warning('on','MATLAB:hg:uicontrol:MinMustBeLessThanMax')
warning('on','MATLAB:hg:uicontrol:ValueMustBeInRange')

p.continuous_callback_function = @self.manipulateEvaluate;


