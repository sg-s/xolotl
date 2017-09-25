% test script for matlab wrapper 
% this tests sets up a two-compartment network
% where the first compartment is voltage clamped,
% and has no channels, and the second compartment 
% has just a sodium and a potassium channel
% the two compartments are electrically coupled 
% the goal is to evoke a spike by driving the 
% first compartment with a short pulse



x = xolotl;
x.dt = 50e-3;
x.t_end = 1e3;
V_clamp = 0*(x.dt:x.dt:x.t_end) - 50;
V_clamp(5e3:5.1e3) = 70;
x.addCompartment('C1',-70,0.05,10,0.0628,14.96,3000,0.05,200);
x.V_clamp = V_clamp; % only the first compartment can be clamped 

x.addCompartment('C2',-70,0.01,10,0.0628,14.96,3000,0.05,200);
x.addConductance('C2','prinz/NaV',1000,50);
x.addConductance('C2','prinz/Kd',250,-80);

x.addCompartment('C3',-70,0.01,10,0.0628,14.96,3000,0.05,200);
x.addConductance('C3','prinz/NaV',1001,50);
x.addConductance('C3','prinz/Kd',250,-80);


x.addSynapse('Elec','C1','C2',20); % one-way electrical syanpse from 1->2
x.addSynapse('Elec','C1','C3',20); % one-way electrical syanpse from 1->3

x.compile;

if usejava('jvm')
	% plot(V)
	x.manipulate;
else
	disp('voltage clamp test passed!')
end



% x = xolotl;
% x.dt = 50e-3;
% x.t_end = 1e3;
% V_clamp = 0*(x.dt:x.dt:x.t_end) - 50;
% V_clamp(5e3:5.1e3) = 70;
% V_clamp(6e3:6.1e3) = 70;
% x.addCompartment('C1',-70,0.05,10,0.0628,14.96,3000,0.05,200);
% x.V_clamp = V_clamp; % only the first compartment can be clamped 

% x.addCompartment('C2',-70,0.01,10,0.0628,14.96,3000,0.05,200);
% x.addConductance('C2','prinz/NaV',1000,50);
% x.addConductance('C2','prinz/Kd',250,-80);
% % x.addConductance('C2','lin/NaT',6,45);
% % x.addConductance('C2','lin/Kslow',250,-80);

% x.addSynapse('Elec','C1','C2',20); % one-way electrical syanpse from 1->2

% x.compile;


% [V,Ca,I_clamp,C] = x.integrate;

% assert(~any(isnan(V(:))),'V contains NaNs, probably something wrong')

% assert(~any(isnan(Ca(:))),'Ca contains NaNs, probably something wrong')


