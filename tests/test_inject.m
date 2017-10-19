% test for I_ext
% makes a f-I curve of a simple HH type neuron

vol = 1; % this can be anything, doesn't matter
f = 14.96; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;


x = xolotl;
x.closed_loop = false;
x.dt = 50e-3;
x.t_end = 2e3;
I_ext = 0*(x.dt:x.dt:x.t_end);
I_steps = logspace(-2,1,30);
x.addCompartment('C1',-70,0.05,10,0.0628,vol, phi, 3000,0.05,tau_Ca,0);
% x.addConductance('C1','prinz/NaV',1000,50);
% x.addConductance('C1','prinz/Kd',250,-80);

x.addConductance('C1','liu/NaV',1830,30);
x.addConductance('C1','liu/ACurrent',246,-80);
x.addConductance('C1','liu/KCa',980,-80);
x.addConductance('C1','liu/Kd',610,-80);
x.addConductance('C1','liu/HCurrent',10.1,-20);
x.addConductance('C1','Leak',.99,-50);



if usejava('jvm')
	%
	figure, hold on
	for i = 1:length(I_steps)
		I_ext(:) = I_steps(i);
		x.I_ext = I_ext;
		V = x.integrate;
		n_spikes = length(computeOnsOffs(V>0));
		plot(I_steps(i),n_spikes,'k+')
	end

	xlabel('Injected current (nA)')
	ylabel('# of spikes')

	prettyFig();
	

else
	I_ext(:) = I_steps(1);
	x.I_ext = I_ext;
	V = x.integrate;
	disp('voltage clamp test passed!')
end

