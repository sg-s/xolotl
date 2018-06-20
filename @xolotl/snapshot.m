%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: create a snapshot of the model's current state
% 
function snapshot(self, snap_name)

assert(nargin == 2, 'Two arguments required')
assert(ischar(snap_name),'snap_name should be a string')

S = self.serialize;

rm_these = find(strcmp(snap_name,{self.snapshots.name}));
self.snapshots(rm_these) = [];

self.snapshots = [self.snapshots; struct('name',snap_name,'V',S)];

