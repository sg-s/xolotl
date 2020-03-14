% measures metrics
% and returns data

function [PrimaryOutput, metrics] = sampleMetrics(x, PrimaryOutputName, parameters, SamplePoint)

SamplePoint = SamplePoint(:);
SamplePoint = SamplePoint';

corelib.assert(isa(x,'xolotl'),'First argument should be a xolotl object')

if isempty(PrimaryOutputName)
	PrimaryOutputName = 'burst_period';
end

corelib.assert(exist('parameters','var') == 1,'parameters not defined')

corelib.assert(exist('SamplePoint','var') == 1,'SamplePoint not defined')

corelib.assert(size(SamplePoint,2) == length(parameters),'Size of SamplePoint and parameters dont match')

try
	x.get(parameters);
catch err
	error('Parameter names cannot be resolved in the xolotl object provided')
end



x.set(parameters,SamplePoint)
x.integrate;
V = x.integrate;
metrics = xtools.V2metrics(V,'sampling_rate',round(1/x.dt),'ibi_thresh',100);


PrimaryOutput = vertcat(metrics.(PrimaryOutputName));
