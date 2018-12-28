% this test should test xolotl.add
function testXolotlAdd(self)


x = self.single_comp;

% make sure we can't create a compartment without a label
self.assertError(@() x.add('compartment'),'xolotl:add:no_label')

% check that the name is actually a legal name
illegal_names = x.illegal_names;
for i = 1:length(illegal_names)
	self.assertError(@() x.add('compartment',illegal_names{i}),'xolotl:add:illegal_name')
end


% check that the object being added is a compartment
self.assertError(@() x.add('NaV','wow'),'xolotl:add:not_compartment')
self.assertError(@() x.add(cpplab('NaV'),'wow'),'xolotl:add:not_compartment')

% check that I_ext and V_clamp are reset when we add a compartment
x.add('compartment','wow');
self.assertTrue(size(x.I_ext,2) == 2,'I_ext not correctly reset')

x.add('compartment','wow2');
self.assertTrue(size(x.V_clamp,2) == 3,'V_clamp not correctly reset')

% check that properties that we specify using Name-Value syntax
% stick in the new compartment 
x.add('compartment','wow3','A',123);
self.assertTrue(x.wow3.A == 123, 'Compartment props incorrectly set')