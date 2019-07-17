%                          _                                       
%                         | |                                      
%     _ __  ___ _   _  ___| |__   ___  _ __   ___  _ __ ___  _ __  
%    | '_ \/ __| | | |/ __| '_ \ / _ \| '_ \ / _ \| '_ ` _ \| '_ \ 
%    | |_) \__ \ |_| | (__| | | | (_) | |_) | (_) | | | | | | |_) |
%    | .__/|___/\__, |\___|_| |_|\___/| .__/ \___/|_| |_| |_| .__/ 
%    | |         __/ |                | |                   | |    
%    |_|        |___/                 |_|                   |_|
%  
% gathers data from all simulations running on
% local and remote clusters 


function [all_data, all_params, all_param_idx] = gather(self)
	
% make sure nothing is running on any cluster
% for i = 1:length(self.clusters)
% 	if strcmp(self.clusters(i).Name,'local')
% 		[n_do, n_doing] = self.getJobStatus;
% 		assert(n_do == 0,'At least one job is still queued on local')
% 		assert(n_doing == 0,'At least one job is still running on local')
% 	else
% 		plog = self.getRemoteState(i);
% 		assert(plog.n_do == 0,'At least one job is still queued on remote')
% 		assert(plog.n_doing == 0,'At least one job is still running on remote')
% 	end
	
% end

% copy all files from all remotes onto local, and gather locally 
for i = 1:length(self.clusters)
	if strcmp(self.clusters(i).Name,'local')
	else
		disp(['Copying data from ' self.clusters(i).Name])


		% copy them in 10 blocks so that we can report progress...
		for j = 1:10
			corelib.textbar(j,10)
			[e,o] = system(['scp ' self.clusters(i).Name ':~/.psych/done/job_' strlib.oval(j) '*.ppp*  ~/.psych/done/' ]);
			if any(any(strfind(o,'No such file or directory')))
				continue
			end

			assert(e == 0,'Error copying job file to remote cluster')
		end

	end
end

done_folder = [self.xgrid_folder filesep 'done' filesep ];
job_files =  dir([done_folder '*.ppp']);
data_files =  dir([done_folder '*.ppp.data']);
assert(length(job_files) == length(data_files),'# of data files does not match # of job files')


load([done_folder data_files(1).name],'-mat');
all_data = data;
load([done_folder job_files(1).name],'-mat');
all_params = this_params;
all_param_idx = param_idx;

xhash0 = xhash;

for i = 2:length(data_files) % because we've already loaded the first one (see above)
	corelib.textbar(i,length(data_files))
	clear data param_idx param_names this_params xhash
	load([done_folder data_files(i).name],'-mat');
	load([done_folder job_files(i).name],'-mat');

	% check that the xhash matches
	assert(strcmp(xhash,xhash0),'xolotl hashes for two files do not match. this is a fatal error')

	for j = 1:length(data)
		if size(data{j},2) ~= size(this_params,2)
			error('Size of params and data do not match. this is a fatal error')
		end
	end

	for j = 1:length(all_data)
		all_data{j} = [all_data{j} data{j}];
	end

	all_params = [all_params this_params];
	all_param_idx = [all_param_idx param_idx];
end
