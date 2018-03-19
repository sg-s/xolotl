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
A_soma = .0628; % using Prinz's value
vol_soma = A_soma;
A_axon = .01; % make the axon a little smaller
vol_axon = A_axon;
phi_soma = (2*f*F*vol_soma)/tau_Ca;
phi_axon = (2*f*F*vol_axon)/tau_Ca;

x = xolotl;
x.add('AB','compartment','V',-65,'Ca',0.02,'Cm',c_m,'A',0.0628,'vol',vol_soma,'phi',phi_soma,'Ca_out',Ca_ext,'Ca_in',Ca_int,'tau_Ca',tau_Ca);


x.add('Axon',,-60,0.02,c_m,A_axon,1,1,1,1,1,0);

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

% connect axon to soma
x.connect('AB','Axon',30);

x.t_end = 20e3;

x.t_end = 5e3;
x.closed_loop = false;
x.manipulate



