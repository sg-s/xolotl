% s function for use with single compartments

function [firing_rate, isis, all_isis, Ca, areas] = measureMetrics(x,~,~)



x.reset;
x.integrate;

[V, Ca] = x.integrate;


firing_rate = xtools.findNSpikes(V)/(x.t_end*1e-3);

all_isis = diff(xtools.findNSpikeTimes(V,1e3+1))*1e-3*x.dt;
isis(1) = mean(all_isis);
isis(2) = std(all_isis);

Ca = [mean(Ca); std(Ca)];

comp_names =  x.Children;
for i = length(comp_names):-1:1
	areas(i) = x.(comp_names{i}).A;
end

