% sets up a xfit object to find half-center
% oscillators

function p = setup()

x = xolotl.examples.networks.HCO;
x.integrate;

p = xfit;
p.x = x;

p.SimFcn = @HCO.cost;

p.ShowFcn = @HCO.show;

FitParameters = [x.find('Cell1*gbar'); x.find('*Cell2.gmax'); 'Cell1.GradedCell2.Vth'; 'Cell1.HCurrent.Vhalf'];



other_params = cellfun(@(x) strrep(x,'Cell1','Cell2'),FitParameters,'UniformOutput',false);
other_params = cellfun(@(x) strrep(x,'GradedCell2','GradedCell1'),other_params,'UniformOutput',false);

p.FitParameters = FitParameters;
p.SaveParameters = [FitParameters; other_params];

p.data.FitParameters = FitParameters;
p.data.other_params = other_params;

% configure bounds
%       A    CaS CaT  H    KCa   Kd    Leak NaV   gmax   synapse_Vth    H_Vth
p.lb = [10   10   10   10   10   900   1    900  100      -80          -90];
p.ub = [1e3  100  10  10  300   2e3   10    3e3  100     -35          -50];