%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### snapshot

**Syntax**

```matlab
x.snapshot('snap_name')
```

**Description**

Saves the current state of a ``xolotl`` object for future use. 


!!! warning
    Creating two snapshots with the same name will overwrite the first. 


**Example**

```
% assuming a xolotl object is set up
x.integrate;
x.snapshot('base');
x.set('*gbar') = 1e-3; % turn off all conductances
x.integrate;
% now go back to original state
x.reset('base')
```
	

!!! info "See Also"
    ->xolotl.reset
    * [How to: save configurations and use snapshots](https://xolotl.readthedocs.io/en/master/how-to/snapshots/)

%}


function snapshot(self, snap_name)

assert(nargin == 2, 'Two arguments required')
assert(ischar(snap_name),'snap_name should be a string')

S = self.serialize;

rm_these = find(strcmp(snap_name,{self.snapshots.name}));
self.snapshots(rm_these) = [];

self.snapshots = [self.snapshots; struct('name',snap_name,'V',S)];

