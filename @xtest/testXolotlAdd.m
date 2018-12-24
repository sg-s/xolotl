% this test should test xolotl.add
function testXolotlAdd(self)

self.createSingleCompartment()

% make sure we can't create a compartment without a label
self.assertError(@() self.x.add('compartment'),'xolotl:add:no_label')

% check that the name is actually a legal name
illegal_names = self.x.illegal_names;
for i = 1:length(illegal_names)
	self.assertError(@() self.x.add('compartment',illegal_names{i}),'xolotl:add:illegal_name')
end


% check that the object being added is a compartment
self.assertError(@() self.x.add('NaV','wow'),'xolotl:add:not_compartment')
self.assertError(@() self.x.add(cpplab('NaV'),'wow'),'xolotl:add:not_compartment')

% check that I_ext and V_clamp are reset when we add a compartment


% check that properties that we specify using Name-Value syntax
% stick in the new compartment 