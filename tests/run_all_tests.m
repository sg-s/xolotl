

function run_all_tests(cleanup)

if nargin < 1
	cleanup = false;
end

if cleanup
	xolotl.cleanup;
end


all_tests = dir('test*.m');	
	
for i = length(all_tests):-1:1	
	disp(['Running test: ' all_tests(i).name])	
	try	
		[~,script_name] = fileparts(all_tests(i).name);
		evalin('base',script_name);
		pause(1)	
		close all	
		% clearvars -except all_tests i 	
		cprintf('green','TEST PASSED\n')	
	catch err
		cprintf('red','TEST FAILED\n')	
	end	
end 