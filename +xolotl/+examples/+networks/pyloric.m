

%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

## pyloric

**Syntax**

```matlab
x = xolotl.examples.networks.pyloric();
```

**Description**

makes a three-cell model of the pyloric network
following Prinz et al. Nat. Neuro. 


%}


function x = pyloric()


% the order here doesn't matter -- everything will be re-arranged
% alphabetically 
channels = {'prinz/NaV','prinz/CaT','prinz/CaS','prinz/ACurrent','prinz/KCa','prinz/Kd','prinz/HCurrent','Leak'};


x = xolotl;

x.add('compartment','AB');
x.AB.add('prinz/CalciumMech');

for i = 1:length(channels)
	x.AB.add(channels{i});
end

x.add(copy(x.AB),'LP')
x.add(copy(x.AB),'PY')

% configure gbars
x.AB.set('*gbar', [500,  60,  25,   .1,   50, 1000,   0,   1000])
x.LP.set('*gbar', [200,  40,   0,   .5,   0,  250,    0.3, 1000])
x.PY.set('*gbar', [500,  20,  24,   .5,   0,  1250,   0.1, 1000])

x.set('*Leak.E',-50)


% set up synapses as in Fig. 2e
x.connect('AB','LP','Chol','gmax',30);
x.connect('AB','PY','Chol','gmax',3);
x.connect('AB','LP','Glut','gmax',30);
x.connect('AB','PY','Glut','gmax',10);
x.connect('LP','PY','Glut','gmax',1);
x.connect('PY','LP','Glut','gmax',30);
x.connect('LP','AB','Glut','gmax',30);

x.t_end = 5e3;