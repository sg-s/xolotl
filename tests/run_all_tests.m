all_tests = dir('test*.m');

for i = length(all_tests):-1:1
	disp(['Running test: ' all_tests(i).name])
	try
		run(all_tests(i).name)
		pause(1)
		close all
		clearvars -except all_tests i 
		cprintf('green','TEST PASSED\n')
	catch
		cprintf('red','TEST FAILED\n')
	end
end