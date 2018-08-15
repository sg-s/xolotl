%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

findNSpikes
^^^^^^^^^^^

static method of ``xolotl`` that computes the number of spikes in a voltage trace. Example use ::

   N = xolotl.findNSpikes(V);
   N = xolotl.findNSpikes(V, on_off_thresh)

``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0 mV. 

See Also
--------

- xolotl.findNSpikeTimes


%}


function f = findNSpikes(V, on_off_thresh)

assert(isvector(V),'V should be a vector')
assert(~any(isnan(V)),'V cannot contain NaNs')
assert(~any(isinf(V)),'V cannot contain Infs')

if nargin < 2
	on_off_thresh = 0;
end

assert(isscalar(on_off_thresh),'on_off_thresh must be a scalar')

V = V(:);

[ons, offs] = computeOnsOffs(V > on_off_thresh);
if isempty(offs) || isempty(ons)
	f = 0;
	return
end

f = length(ons);