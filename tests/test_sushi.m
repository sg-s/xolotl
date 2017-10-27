% tests sushi-bar model of mrna trafficking 
% also tests slicing a compartment to make a section 
% also a test of the soto-trevino conductances

% Soto-Trevino parameters
f = 0.418; % uM/nA
C_soma = 9; % nF
C_axon = 1.5; 
g_axial = .3; % uS
Ca_ext = 13e3; % uM
Ca_int = .5; % uM

tau_Ca = 303;
F = 96485; % Faraday constant in SI units


c_m = 10; % nF/mm^2
Ca_target = 3.3; % used only when we add in homeostatic control 
A_soma = .0628; % using Prinz's value
vol_soma = A_soma;
A_axon = .01; % make the axon a little smaller
vol_axon = A_axon;
phi_soma = (2*f*F*vol_soma)/tau_Ca;
phi_axon = (2*f*F*vol_axon)/tau_Ca;

x = xolotl;
x.addCompartment('AB',-60,.1,c_m,A_soma,A_soma,24,Ca_ext,Ca_int,tau_Ca,Ca_target);


x.addCompartment('Axon',-60,0.02,c_m,A_axon,1,1,1,1,1,0);

% add axon conductances 
x.addConductance('Axon','soto-trevino/NaV',2000,50);
x.addConductance('Axon','soto-trevino/Kd',350,-80);
x.addConductance('Axon','Leak',.012,-60);

% add soma conductances
x.addConductance('AB','soto-trevino/CaTAB',80,30);
x.addConductance('AB','soto-trevino/CaS',40,30);
x.addConductance('AB','soto-trevino/NaP',3,50);
x.addConductance('AB','soto-trevino/HCurrent',.2,-20);
x.addConductance('AB','soto-trevino/Kd',2000,-80);
x.addConductance('AB','soto-trevino/KCaAB',1000,-80);
x.addConductance('AB','soto-trevino/ACurrentAB',200,-80);
x.addConductance('AB','Leak',.005,-50);


% make a neurite with 10 segments
x.copy('AB','Neurite');
x.slice('Neurite',10,g_axial);
x.connect('AB','NeuriteS1',g_axial); 

% connect axon to soma
g_axial = 30;
x.connect('AB','Axon',g_axial);

% set up some relational parameters
x.AB.vol = @() x.AB.A;

x.closed_loop = false;


% add a sushi belt controller  
Alpha = 1e-4;
Beta = Alpha*(.9);
Gamma = Beta/50;
tau_g = 1e3;

G = x.getConductances('AB');
G = G(1)./(G);
G = G*1e3;

x.addSushiController('CaTAB','AB','Neurite',G(1),tau_g,Alpha,Beta,Gamma)
x.addSushiController('CaS','AB','Neurite',G(2),tau_g,Alpha,Beta,Gamma)
x.addSushiController('NaP','AB','Neurite',G(3),tau_g,Alpha,Beta,Gamma)
x.addSushiController('HCurrent','AB','Neurite',G(4),tau_g,Alpha,Beta,Gamma)
x.addSushiController('Kd','AB','Neurite',G(5),tau_g,Alpha,Beta,Gamma)
x.addSushiController('KCaAB','AB','Neurite',G(6),tau_g,Alpha,Beta,Gamma)
x.addSushiController('ACurrentAB','AB','Neurite',G(7),tau_g,Alpha,Beta,Gamma)


x.cleanup;
x.transpile;
x.compile;
x.dt = 100e-3;
x.t_end = 100e3;
[V,Ca] = x.integrate;


if usejava('jvm')

	plot(V)


	% % plot mRNA profiles
	% xx = 1:length(x.compartment_names);

	% figure, hold on
	% for i = 1:7
	% 	a = length(x.compartment_names)*(i-1) + 1;
	% 	z = a + length(x.compartment_names) - 1;
	% 	yy = cellfun(@(x) x.m, x.controllers(a:z));
	% 	plot(xx,yy)
	% end
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

