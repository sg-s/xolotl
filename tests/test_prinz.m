% tests a neuron that reproduces neurons from the Prinz database 

if ~exist('z','var')

		
	r0 = 0.0707; % mm
	shell_thickness = 55e-3; % mm

	vol = @(r) (4.*pi.*r.*r.*shell_thickness);
	A = @(r) (4.*pi.*r.*r);

	f = 14.96; % uM/nA
	tau_Ca = 200;
	F = 96485; % Faraday constant in SI units
	phi = @(v) ((2*f*F*v)/tau_Ca);
	Ca_target = 7; % used only when we add in homeostatic control 
	

	z = zoidberg;
	G = z.findNeurons('burster');
end

idx = randi(length(G));

x = xolotl;
x.addCompartment('AB',-60,0.02,10,A(r0),vol(r0),phi(vol(r0)),3000,0.05,tau_Ca,0);


x.addConductance('AB','prinz/NaV',G(1,idx),50);
x.addConductance('AB','prinz/CaT',G(2,idx),30);
x.addConductance('AB','prinz/CaS',G(3,idx),30);
x.addConductance('AB','prinz/ACurrent',G(4,idx),-80);
x.addConductance('AB','prinz/KCa',G(5,idx),-80);
x.addConductance('AB','prinz/Kd',G(6,idx),-80);
x.addConductance('AB','prinz/HCurrent',G(7,idx),-20);
x.addConductance('AB','Leak',G(8,idx),-50);

[V,Ca] = x.integrate;

if usejava('jvm')
	figure, hold on
	subplot(2,1,1); hold on
	plot(V)
	ylabel('Voltage (mV)')
	subplot(2,1,2)
	plot(Ca(:,1));
	ylabel('[Ca] (\muM)')
	prettyFig();
else
	disp('Prinz database test passed!')
end