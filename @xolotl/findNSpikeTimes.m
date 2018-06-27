%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

findNSpikeTimes
^^^^^^^^^^^^^^^

static method of ``xolotl`` that returns a vector of spike times from a voltage trace. Spikes are defined as voltage crossings across a threshold. Example use ::

   spiketimes = findNSpikeTimes(V,n_spikes,on_off_thresh);


``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0. ``n_spikes`` is the number of spikes it should look for, and ``spiketimes`` will always be a vector ``n_spikes`` elments long. 

See Also
--------

- xolotl.findNSpikes


%}



function spiketimes = findNSpikeTimes(V,n_spikes,on_off_thresh)

if nargin < 3
	on_off_thresh = 0;
end

spiketimes = NaN(n_spikes,1);
[ons, offs] = computeOnsOffs(V > on_off_thresh);
if isempty(offs) || isempty(ons)
	return
end

stop_here = min([length(ons) n_spikes]);

for j = 1:stop_here
	[~,idx] = max(V(ons(j):offs(j)));
	spiketimes(j) = ons(j) + idx;
end
