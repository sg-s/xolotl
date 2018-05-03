
% in this example, we create three neurons and integrate them together:
% AB is a multi-compartment neuron connected with Axial synpases
% SingleComp is a single compartment model
%
% all neurons have similar parameters for comparison. 

axial_resitivity = 1e-3; % MOhm mm; 

% geometry is a cylinder
r_neurite = .01; % default value
L_neurite = .35; % mm, from Otopalik et al
r_soma = .025;
L_soma = .05; % mm, Jason's guesstimates
phi = 100;

x = xolotl;
x.skip_hash = true;
	% make 3 neurons
	x.add('Soma','compartment','radius',r_soma,'len',L_soma,'phi',phi,'Ca_out',3000,'Ca_in',0.05);
	x.add('Neurite','compartment','radius',r_neurite,'len',L_neurite,'phi',phi,'Ca_out',3000,'Ca_in',0.05);

	prefix = 'prinz/';
	channels = {'ACurrent','CaS','CaT','HCurrent','KCa','Kd','NaV'};
	g =           [500;      60;   25 ;   .1;      50;   1e3;  1e3];
	E =           [-80;      30;   30;   -20;     -80;   -80;  50 ];

	compartments = x.find('compartment');
	for i = 1:length(channels)
		for j = 1:length(compartments)
			x.(compartments{j}).add([prefix channels{i}],'gbar',g(i),'E',E(i));

		end
	end

	x.Soma.NaV.gbar = 0;

	x.slice('Neurite',10,axial_resitivity);

	% comp_names = x.find('compartment');
	% start_axon = 3;
	% for i = 1:start_axon
	% 	x.(comp_names{i}).NaV.gbar = 0;
	% end

	x.connect('Neurite','Soma','Axial','resistivity',axial_resitivity);
	x.connect('Soma','Neurite','Axial','resistivity',axial_resitivity);
	x.Soma.tree_idx = 0;

x.skip_hash = false; x.sha1hash;


x.dt = .1;
x.sim_dt = .01;
x.t_end = 5e3;

% x.t_end = 1;
% x.integrate;
% return

V = x.integrate;

figure('outerposition',[300 300 1200 900],'PaperUnits','points','PaperSize',[1200 900]); hold on
subplot(2,1,1); hold on
time = 1e-3*(1:length(V))*x.dt;
plot(time,V(:,end-1),'r')
set(gca,'YLim',[-80 40])
ylabel('V_{terminal} (mV)')
subplot(2,1,2); hold on
plot(time,V(:,end),'k')
set(gca,'YLim',[-80 40])
xlabel('Time (s)')
ylabel('V_{soma} (mV)')

prettyFig('plw',1.5,'lw',1.5);