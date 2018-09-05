% this script tests whether xolotl's built in
% compartment_name checker works


x = xolotl;

x.add('compartment','AB')


assert(x.checkCompartmentName('PD'),'Cannot add a compartment called "PD". Why?')


% test adding something illegal 
try
	x.add('compartment','compartment')
	error('successfully added a compartment with an illegal name. This should have thrown an error.')
catch err
	disp('Successfully caught error:')
	disp(err.message)
end

% test transpiling and compiling
x.transpile;
x.compile;