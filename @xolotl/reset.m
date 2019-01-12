%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### reset





**Syntax**

```matlab
x.reset()
x.reset('snap_name')
```

**Description**

Resets a xolotl object to some default state.

reset called without any arguments resets the model as best as it can -- voltages are set to -60 mV, Calcium in every compartment is set to the internal value, and the gating variables of every conductance are reset.

`reset` can also be called with a string argument, which is the name of a snapshot previously stored in the model object. Then, `reset` reconfigures the parameters of the model to match that snapshot. This is useful for working with a model, changing parameters, evolving it, and then coming back to where you started off from.

Here's an example:

```
% assuming a xolotl object is set up
x.integrate;
x.snapshot('base');
x.set('*gbar') = 1e-3; % turn off all conductances
x.integrate;
% now go back to original state
x.reset('base')
```

This method supports tab completion. You should be able to
press tab and get a list of snapshots that you want to
reset to.

!!! info "See Also"
    ->xolotl.snapshot

%}


function reset(self, snap_name)

if nargin == 1

	% reset all compartments
	% reset the Ca_average of every compartment
	all_compartments = self.find('compartment');
	for i = 1:length(all_compartments)
		self.(all_compartments{i}).V = -60;
		self.(all_compartments{i}).Ca = .05;
		self.(all_compartments{i}).Ca_average = NaN;
	end

	% reset all conductances
	all_channels = self.find('conductance');

	for i = 1:length(all_channels)
		try
			self.set([all_channels{i} '.m'],0)
		catch
		end
		try
			self.set([all_channels{i} '.h'],1)
		catch
		end
	end

	% reset all synapses
	all_synapses = self.find('synapse');
	for i = 1:length(all_synapses)
		try
			self.get(all_synapses{i}).s = 0;
		catch
		end
	end

	% reset all controllers
	all_controllers = self.find('controller');
	for i = 1:length(all_controllers)
		self.set([all_controllers{i} '.m'],0)
	end

else

	% check if this snap exists
	idx = find(strcmp(snap_name,{self.snapshots.name}),1,'first');
	assert(~isempty(idx),'snapshot not found')
	self.deserialize(self.snapshots(idx).V);


end
