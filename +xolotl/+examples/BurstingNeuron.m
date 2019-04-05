
%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

## BurstingNeuron

**Syntax**

```matlab
x = xolotl.examples.BurstingNeuron('liu');
x = xolotl.examples.BurstingNeuron('prinz');
```

**Description**

makes a single-compartment neuron that bursts, using
channels from Liu et al. or Prinz et al. 


%}


function x = BurstingNeuron(varargin)


options.prefix = 'prinz';
options.CalciumMech = 'prinz';
options.f = [];

options = corelib.parseNameValueArguments(options, varargin{:});

if isempty(options.f) && strcmp(options.prefix,'prinz')
	options.f = 14.96;
elseif isempty(options.f) && strcmp(options.prefix,'liu')
	options.f = 1.496;
end


x = xolotl;
A = 0.0628;
vol = A;
x.add('compartment','AB','A',A,'vol',vol);

switch options.CalciumMech
case 'prinz'

	x.AB.add('prinz/CalciumMech','f',options.f);
case 'bucholtz'
	phi = options.f*1.9297e+05*vol/200;
	x.AB.add('bucholtz/CalciumMech','phi',phi);
otherwise
	error('Unknown Calcium Mechanism')
end

prefix = options.prefix;
switch prefix

case 'liu'
	x.AB.add([prefix '/NaV'],'gbar',@() 115/x.AB.A,'E',30);
	x.AB.add([prefix '/CaT'],'gbar',@() 1.44/x.AB.A,'E',30);
	x.AB.add([prefix '/CaS'],'gbar',@() 1.7/x.AB.A,'E',30);
	x.AB.add([prefix '/ACurrent'],'gbar',@() 15.45/x.AB.A,'E',-80);
	x.AB.add([prefix '/KCa'],'gbar',@() 61.54/x.AB.A,'E',-80);
	x.AB.add([prefix '/Kd'],'gbar',@() 38.31/x.AB.A,'E',-80);
	x.AB.add([prefix '/HCurrent'],'gbar',@() .6343/x.AB.A,'E',-20);
	x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);
case 'prinz'
	channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};

	gbar(:,1) = [1000 25  60 500  50  1000 .1];
	E =         [50   30  30 -80 -80 -80   -20];


	for i = 1:length(channels)
		x.AB.add([prefix filesep channels{i}],'gbar',gbar(i),'E',E(i));
	end


end



x.t_end = 2e3;
x.integrate;