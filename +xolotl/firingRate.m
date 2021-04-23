% measures the firing rate of all compartments in the model

function FR = firingRate(x)

arguments
	x (1,1) xolotl
end

output_type = x.output_type;
x.output_type = 0;

V = x.integrate;

x.output_type = output_type;

FR = NaN(size(V,2),1);
for i = 1:length(FR)
	FR(i) = xtools.findNSpikes(V(:,i));
end

FR = FR/x.t_end/1e-3;