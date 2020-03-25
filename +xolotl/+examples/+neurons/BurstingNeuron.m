
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


x = xolotl;
A = 0.0628;
vol = A;
x.add('compartment','AB','A',A,'vol',vol);


prefix = options.prefix;


channels = {'ACurrent','HCurrent','KCa','Kd','CaS','CaT','NaV'};


for i = 1:length(channels)
	try
		x.AB.add([prefix filesep channels{i}])
	catch
		% fall back onto Prinz channels
		x.AB.add(['prinz/' channels{i}])
	end
end

x.AB.add('Leak');








if any(strfind(prefix,'prinz'))
	prefix = 'prinz';
elseif any(strfind(prefix,'liu'))
	prefix = 'liu';
end




x.AB.add([options.CalciumMech '/CalciumMech']);


if isprop(x.AB.CalciumMech,'f') && any(strfind(options.prefix,'liu'))
	x.AB.CalciumMech.f = 1.496;
end




% attempt to configure gbars correctly
switch prefix

case 'liu'
	x.AB.NaV.gbar = 1831;
	x.AB.CaT.gbar = 23;
	x.AB.CaS.gbar = 27;
	x.AB.ACurrent.gbar = 246;
	x.AB.KCa.gbar = 980;
	x.AB.Kd.gbar = 610;
	x.AB.HCurrent.gbar = 10.1;
	x.AB.Leak.gbar = .99;
	x.AB.Leak.E = -50;
case 'prinz'
	channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};

	gbar(:,1) = [1000 25  60 500  50  1000 .1];

	for i = 1:length(channels)
		x.AB.(channels{i}).gbar = gbar(i);
	end

case 'golowasch'
	channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};

	gbar(:,1) = [1000 25  60 500  50  1000 .1];

	for i = 1:length(channels)
		x.AB.(channels{i}).gbar = gbar(i);
	end

end



x.t_end = 3e3;
x.integrate;