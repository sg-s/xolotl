% this helper function measures
% metrics in a xolotl model for some parameters
% and returns the things it measures
% you can configure it to return one value from all
% possible metrics defined in xtools.V2metrics 
% as the first output

function [PrimaryOutput, metrics] = sampleMetrics(x, ParameterNames, ParameterValues, options)


arguments
	x (1,1) xolotl
	ParameterNames cell
	ParameterValues double 
	options.PrimaryOutputName char = 'burst_period'

end

ParameterValues = ParameterValues(:);
ParameterValues = ParameterValues';


try
	x.get(ParameterNames);
catch err
	error('Parameter names cannot be resolved in the xolotl object provided')
end



x.set(ParameterNames,ParameterValues)
x.integrate;
V = x.integrate;
metrics = xtools.V2metrics(V,'sampling_rate',round(1/x.dt),'ibi_thresh',100);


PrimaryOutput = metrics.(options.PrimaryOutputName);
