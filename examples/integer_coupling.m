
x = xolotl;


% make Int1
x.add('compartment','Int1','radius',.025,'len',.05);
x.add('compartment','Int1Neurite','len',1,'radius',1.25);
x.add('compartment','Int1Axon','len',1,'radius',1.25);

% add channels to Int1 
x.Int1Axon.add('nadim98/GenericChannel','NaV','gbar',35,'E',45,'p',3,'km',-.08,'Vkm',-26,'lm',0,'Vlm',0,'tau1m',1,'tau2m',0,'q',1,'kh',.13,'Vkh',-38,'lh',-.12,'Vlh',-67,'tau1h',0,'tau2h',5);
x.Int1Axon.add('nadim98/GenericChannel','Kd','gbar',60,'E',-80,'p',4,'km',-.045,'Vkm',-25,'lm',-.065,'Vlm',-35,'tau1m',4,'tau2m',150,'q',0,'kh',0,'Vkh',0,'lh',0,'Vlh',0,'tau1h',0,'tau2h',0);
x.Int1Axon.add('nadim98/GenericChannel','HCurrent','gbar',20,'E',10,'p',1,'km',2,'Vkm',-65,'lm',2,'Vlm',-65,'tau1m',200,'tau2m',2500,'q',0,'kh',0,'Vkh',0,'lh',0,'Vlh',0,'tau1h',0,'tau2h',0);
x.Int1Axon.add('Leak','gbar',0.073,'E',-30)

x.Int1.add('Leak','gbar',1,'E',-40)
x.Int1Neurite.add('Leak','gbar',1,'E',-40)

return

%Add conductances
conds = {'prinz/NaV','prinz/Kd','prinz/HCurrent','prinz/ACurrent','prinz/CaT','prinz/CaS','prinz/KCa','Leak'};
% AB
gbars(:, 1) = [0, 650, 0.5, 500, 25, 100, 30, 0];
% ABneurite
gbars(:, 2) = [1000, 500, 0, 200, 0, 0, 0, 0.5];

% Int1
gbars(:, 3) = [0,  500, 0.5, 0,  0, 0, 0, 0];
% Int1neurite
gbars(:, 4) = [3000,  800, 1, 0,  0,  0, 0, 0.3];

% LG
%gbars(:, 5) = [0, 0, 0, 0,  0, 0, 0, 0];
% LGneurite
%gbars(:, 6) = [0, 0, 0, 0,  0, 0, 0, 0.3];

%Assign conductances
comps       = x.find('compartment');
for i = 1:length(comps)
    for j = 1:length(conds)
        x.(comps{i}).add(conds{j}, 'gbar', gbars(j, i));
    end
end

%Enables a Calcium Mechanic
for i = 1:length(comps)
    x.(comps{i}).add('prinz/CalciumMech');
end

%AB dimensions
x.AB.radius = 0.025;
x.AB.len = 0.05;
x.ABneurite.radius = 0.01;
x.ABneurite.len = 0.35*5;

%Int1 dimensions
x.Int1.radius = 0.025;
x.Int1.len = 0.05;
x.Int1neurite.len = 0.35*5;
x.Int1neurite.radius = 0.01;

%LG dimensions
%x.LG.len = 0.1;
%x.LG.radius = 0.025;
%x.LGneurite.len = 0.35*5;
%x.LGneurite.radius = 0.01;

%Connecting compartments
x.slice('ABneurite',2);
x.connect('ABneurite1','AB');
x.AB.tree_idx = 0;

x.slice('Int1neurite',2);
x.connect('Intneurite1','Int1');
x.Int1.tree_idx = 0;
%x.slice('LGneurite',2);
%x.connect('LGneurite1','LG');
%x.LG.tree_idx = 0;

%Connects neurons (synapses)
x.connect('ABneurite2','Intneurite1','prinz/Glut','gmax',30);
%x.connect('Intneurite1','LGneurite1','prinz/Glutamate','gmax',10);
%x.connect('LGneurite1','Intneurite1','prinz/Glutamate','gmax',10);

%time base for run (ms)
x.t_end = 1e4; 

