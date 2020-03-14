%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### gather
%
%
% **Syntax**
%
% ```matlab
% 	[all_data, all_params, all_params_idx] = p.gather()
% ```
%
% **Description**
%
% Collects together all results from all remote and local clusters.
% `all_data` is a cell array where the elements are the outputs from each output `p.sim_func`.
% `all_params` is an M x N matrix, where M is the number of parameters, and N is the number of simulations.
% `all_params_idx` is a linear index through `all_params`.
%
% **Technical Details**
%
% The dimensions of `all_params` are identical to the `params` input of `batchify`.
% Despite this, the matrices are not identical.
% The `all_params` matrix is shuffled, due to the nature of performing the simulations in parallel.
%
% See Also:
% xgrid.batchify


function [all_data, all_params, all_param_idx] = gather(self)

% make sure nothing is running on any cluster
% for i = 1:length(self.clusters)
% 	if strcmp(self.clusters(i).Name,'local')
% 		[n_do, n_doing] = self.getJobStatus;
% 		corelib.assert(n_do == 0,'At least one job is still queued on local')
% 		corelib.assert(n_doing == 0,'At least one job is still running on local')
% 	else
% 		plog = self.getRemoteState(i);
% 		corelib.assert(plog.n_do == 0,'At least one job is still queued on remote')
% 		corelib.assert(plog.n_doing == 0,'At least one job is still running on remote')
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

			corelib.assert(e == 0,'Error copying job file to remote cluster')
		end

	end
end

done_folder = [self.xgrid_folder filesep 'done' filesep ];
job_files =  dir([done_folder '*.ppp']);
data_files =  dir([done_folder '*.ppp.data']);
corelib.assert(length(job_files) == length(data_files),'# of data files does not match # of job files')


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
	corelib.assert(strcmp(xhash,xhash0),'xolotl hashes for two files do not match. this is a fatal error')

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
