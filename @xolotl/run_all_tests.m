
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% ### run_all_tests
%
%
%
% **Syntax**
%
% ```matlab
% xolotl.run_all_tests
% ```
%
% **Description**
%
% A static method that runs all tests (demos/examples)
% in xolotl/examples. If you've just installed this,
% it may be a good idea to run this to make sure everything works.
%
% This method is called during testing, and only if all
% tests pass is a release published.
%



function [passed, total] = run_all_tests(cleanup)

% create the parpool
gcp;

hashlib.test()

if nargin < 1
	cleanup = true;
end

if cleanup
	xolotl.cleanup;
end

% rebuild cpplab cache just to be safe
cpplab.rebuildCache;

this_dir = fileparts(fileparts(which('xolotl.run_all_tests')));
all_tests = dir(pathlib.join(this_dir,'examples','demo*.m'));

cd(pathlib.join(this_dir,'examples'))


passed = 0;
total = length(all_tests);


for i = length(all_tests):-1:1
	fprintf(['Running test: ' all_tests(i).name ' '])
	try
		[~,script_name] = fileparts(all_tests(i).name);
		evalin('base',script_name);
		pause(1)
		close all
		% clearvars -except all_tests i
		corelib.cprintf('green','TEST PASSED\n')
		passed = passed + 1;
	catch err
		corelib.cprintf('red','TEST FAILED\n')

		% also show the error
		disp(err)

	end
end


% now run the actual tests
xt = xtest; run(xt)


% then test all the channels
xolotl.testConductances;