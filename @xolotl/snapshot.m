%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

# snapshot

Saves the current state of a ``xolotl`` object for future use. Usage ::


```
x.snapshot('snap_name')
```

!!! warning
    Creating two snapshots with the same name will overwrite the first. 


Usage:

```
% assuming a xolotl object is set up
x.integrate;
x.snapshot('base');
x.set('*gbar') = 1e-3; % turn off all conductances
x.integrate;
% now go back to original state
x.reset('base')
```
	

### See Also

xolotl.reset

%}


function snapshot(self, snap_name)

assert(nargin == 2, 'Two arguments required')
assert(ischar(snap_name),'snap_name should be a string')

S = self.serialize;

rm_these = find(strcmp(snap_name,{self.snapshots.name}));
self.snapshots(rm_these) = [];

self.snapshots = [self.snapshots; struct('name',snap_name,'V',S)];

