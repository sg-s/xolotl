% this demo shows how we can use xfind
% to sample random models and log them to disk



% set up a neuron
p = xfind;
p.x = xolotl.examples.neurons.BurstingNeuron;




% define parameters to randomly sample
% and bounds
p.ParameterNames = p.x.find('*gbar');
p.Upper = [1e3 200 200 10 500 1e3 10 1e3];
p.Lower = zeros(8,1);


% define the sample function
% here we will use the built-in uniform random sampler
% You can write your own sampler; its job should be 
% to set the parameters of the xolotl model when called
% with zero inputs
p.SampleFcn = @p.uniformRandom;


% define the simulation function
% here we will use the built-in measureFiringRate
% method which does what it says
% You can use your own function; the only 
% requirement is that it returns data, a vector
% of well, data
% Every element of data should contain a scalar double
% and will be logged to disk
p.SimFcn = @p.measureFiringRate;


% say we only we want to keep neurons that are not silent
p.DiscardFcn = @(data) data <= 0;
