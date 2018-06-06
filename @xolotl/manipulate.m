%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: integrates, and generates UI to manipulate parameters
% usage:
%
% assuming x is a xolotl object
% 
% manipualte all parameters:
% x.manipulate() 
%
% manipualte some parameters:
% x.manipulate('some*pattern')
%
% specify your own plot function:
% x.manipulate_plot_func{1} = @some_func
% where some_func accepts a xolotl object as the first argument
% and two other inputs (for I_ext and V_clamp)
% x.manipualte()

function manipulate(self, manipulate_these)


if isempty(self.linked_binary)
	self.transpile;
	self.compile;
end

% disable closed loop
self.closed_loop = false;

[V,Ca] = self.integrate;
time = (1:length(V))*self.dt*1e-3;

t_end = self.t_end;

compartment_names = self.find('compartment');
n = length(compartment_names);


self.manipulate_plot_func{1}(self);




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

else

	if any(strfind(manipulate_these,'*'))
		% first find objects, then get them
		manipulate_these = self.find(manipulate_these);
	end

	real_names = manipulate_these;
	values = NaN*ones(length(real_names),1);
	% check that these exist
	for i = 1:length(real_names)
		assert(self.exist(real_names{i}),'Unknown parameter to manipulate')
		temp = self.get(real_names{i});
		if isa(temp,'function_handle')
			temp = temp();
		end
		values(i) = temp;
	end
end



% semi-intelligently make the upper and lower bounds
lb = values/3;
ub = values*3;


warning('off','MATLAB:hg:uicontrol:ValueMustBeInRange')

% create a puppeteer instance and configure
p = puppeteer(real_names,values,lb,ub,[],true);



p.attachFigure(self.handles.fig);
p.callback_function = @self.manipulateEvaluate;
self.handles.puppeteer_object = p;

