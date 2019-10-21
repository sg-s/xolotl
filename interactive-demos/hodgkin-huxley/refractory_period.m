

global x

if exist('hh_examples.mat','file') == 2
	load('hh_examples.mat')
else

	x = xolotl;
	% x.handles = struct;
	x.add('compartment','AB','A',0.0628,'vol',.0628);


	x.AB.add('GenericKdLike','Kd','gbar',610);
	x.AB.add('Leak','gbar',.99,'E',-50);
	x.AB.add('GenericNaVLike','NaV','gbar',1831,'E',30);

end



x.AB.NaV.h_tau_B1 = .33;


x.closed_loop = true;
x.t_end = 1e3;
x.integrate;
x.t_end = .5e3;

x.dt = .1;
x.sim_dt = .1;


real_names = [x.find('*tau*'); 'pulse_width'; 'pulse_amp_1'; 'pulse_amp_2'; 'pulse_delay'];

values = zeros(length(real_names),1);

for i = 1:length(real_names)
	try
		values(i) = x.get(real_names{i});
	catch
	end
end

global pulse_width;
global pulse_amp_1;
global pulse_amp_2;
global pulse_delay;

% defaults
pulse_amp_1 = 0;
pulse_amp_2 = 0;
pulse_delay = 0;
pulse_width = 0;

% semi-intelligently make the upper and lower bounds
lb = values/3;
ub = values*3;
ub(values==0) = 1;

% create a puppeteer instance and configure
warning('off','MATLAB:hg:uicontrol:ValueMustBeInRange')
warning('off','MATLAB:hg:uicontrol:MinMustBeLessThanMax')
p = puppeteer(real_names,values,lb,ub,[]);

refractory_period_callback;


warning('on','MATLAB:hg:uicontrol:MinMustBeLessThanMax')
warning('on','MATLAB:hg:uicontrol:ValueMustBeInRange')

p.continuous_callback_function = @refractory_period_callback;


