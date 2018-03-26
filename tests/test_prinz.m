% tests a neuron that reproduces neurons from the Prinz database 

if ~exist('z','var')

		
	vol = 1; % this can be anything, doesn't matter
	f = 14.96; % uM/nA
	tau_Ca = 200;
	F = 96485; % Faraday constant in SI units
	phi = (2*f*F*vol)/tau_Ca;


	z = zoidberg;
	G = z.findNeurons('burster');

	x = xolotl;
	x.cleanup;

	x.add('AB','compartment','V',-65,'Ca',0.02,'Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

	idx = randi(length(G));

	x.AB.add('prinz/NaV','gbar',G(1,idx),'E',50);
	x.AB.add('prinz/CaT','gbar',G(2,idx),'E',30);
	x.AB.add('prinz/CaS','gbar',G(3,idx),'E',30);
	x.AB.add('prinz/ACurrent','gbar',G(4,idx),'E',-80);
	x.AB.add('prinz/KCa','gbar',G(5,idx),'E',-80);
	x.AB.add('prinz/Kd','gbar',G(6,idx),'E',-80);
	x.AB.add('prinz/HCurrent','gbar',G(7,idx),'E',-20);
	x.AB.add('Leak','gbar',G(8,idx),'E',-50);


end

idx = randi(length(G));

x.set('AB.NaV.gbar',G(1,idx))
x.set('AB.CaT.gbar',G(2,idx))
x.set('AB.CaS.gbar',G(3,idx))
x.set('AB.ACurrent.gbar',G(4,idx))
x.set('AB.KCa.gbar',G(5,idx))
x.set('AB.Kd.gbar',G(6,idx))
x.set('AB.HCurrent.gbar',G(7,idx))
x.set('AB.Leak.gbar',G(8,idx))

[V,Ca] = x.integrate;

if usejava('jvm')
	close all
	figure, hold on
	subplot(2,1,1); hold on
	plot(V)
	ylabel('Voltage (mV)')
	title(mat2str(G(:,idx)))
	subplot(2,1,2)
	plot(Ca(:,1));
	ylabel('[Ca] (\muM)')
	prettyFig();
	drawnow
else
	disp('Prinz database test passed!')
end