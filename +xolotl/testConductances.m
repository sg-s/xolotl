% tests every conductance in c++/conductance
% and checks that it compiles


xroot = fileparts(fileparts(which('xolotl')));
all_cond = cpplab.search([xroot  filesep 'c++'  filesep 'conductances']);

if exist([xroot filesep 'cond_check.mat'],'file') == 2
	load([xroot filesep 'cond_check.mat'])
else
	warning('It looks like this is the first time conductances are being checked. This will take a while...')
	ok_channels = {};
end




for i = 1:length(all_cond)

	if any(strfind(all_cond{i},'templates')) 
		continue
	end

	H = hashlib.md5hash(cpplab.resolvePath(all_cond{i}),'File');
	if any(strcmp(ok_channels,H))
		continue
	end

	fprintf(all_cond{i});


	x = xolotl;
	x.add('compartment','AB','A',0.0628);



	try
		x.AB.add(all_cond{i},'TestChannel','gbar',10);

		x.integrate;

		corelib.cprintf('green',  ' [OK]\n')
		ok_channels = [ok_channels {H}];

	catch
		corelib.cprintf('red',  ' [FAILED]\n')
		edit(cpplab.resolvePath(all_cond{i}))
	end

	save([xroot filesep 'cond_check.mat'],'ok_channels')
		

end

