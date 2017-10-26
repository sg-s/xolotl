% tests sushi-bar model of mrna trafficking 
% also tests slicing a compartment to make a section 

vol = 1; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 7; % used only when we add in homeostatic control 

x = xolotl;
x.addCompartment('AB',-60,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca,Ca_target);

x.addConductance('AB','liu/NaV',1,30);
x.addConductance('AB','liu/CaT',1,30);
x.addConductance('AB','liu/CaS',1,30);
x.addConductance('AB','liu/ACurrent',1,-80);
x.addConductance('AB','liu/KCa',1,-80);
x.addConductance('AB','liu/Kd',1,-80);
x.addConductance('AB','liu/HCurrent',.1,-20);
x.addConductance('AB','Leak',.99,-50);


% make a neurite with 10 segments
g_axial = 1; % uS (from Soto-Trevino)
gbar_axial = g_axial/(x.AB.A/10);
x.copy('AB','Neurite');
x.slice('Neurite',10,gbar_axial);

% connect the neurite to the soma 
x.connect('AB','NeuriteS1',gbar_axial); 

% add a sushi belt controller  
Alpha = 1e-4;
Beta = Alpha*(.9);
Gamma = Beta/50;
tau_g = 5e3;

x.addSushiController('NaV',666,tau_g,0,Beta,Gamma)
x.addSushiController('CaT',55555,tau_g,Alpha,Beta,Gamma)
x.addSushiController('CaS',45454,tau_g,Alpha,Beta,Gamma)
x.addSushiController('ACurrent',5000,tau_g,Alpha,Beta,Gamma)
x.addSushiController('KCa',1250,tau_g,Alpha,Beta,Gamma)
x.addSushiController('Kd',2000,tau_g,Alpha,Beta,Gamma)
x.addSushiController('HCurrent',125000,tau_g,Alpha,Beta,Gamma)


x.cleanup;
x.transpile;
x.compile;
x.dt = 100e-3;
x.t_end = 20e3;
V = x.integrate;


if usejava('jvm')

	plot(V)


	% plot mRNA profiles
	xx = 1:length(x.compartment_names);

	figure, hold on
	for i = 1:7
		a = length(x.compartment_names)*(i-1) + 1;
		z = a + length(x.compartment_names) - 1;
		yy = cellfun(@(x) x.m, x.controllers(a:z));
		plot(xx,yy)
	end
else
	sp = xolotl.findSpikes(V);
	assert(length(sp) > 100 ,'Not enough spikes!')
	disp('Sushi controller test passed!')
end


% % plot mRNA profiles for NaV as a function of time
% xx = 1:length(x.compartment_names);
% nsteps = 20;
% x.t_end = 2e2;
% figure, hold on
% c = parula(nsteps+1);
% for i = 1:nsteps
% 	x.integrate;
% 	yy = cellfun(@(x) x.m, x.controllers(1:length(x.compartment_names)));
% 	plot(xx,yy,'Color',c(i,:))
% 	drawnow 
% end

