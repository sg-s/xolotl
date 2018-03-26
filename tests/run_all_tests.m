all_tests = dir('test*.m');

for i = 1:length(all_tests)
	disp(['Running test: ' all_tests(i).name])
	try
		run(all_tests(i).name)
		pause(1)
		close all
		clearvars -except all_tests i 
	catch
		disp('Test failed!')
	end
end