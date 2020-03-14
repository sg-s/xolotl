%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### batchify
%
%
% **Syntax**
%
% ```matlab
% 	p.(params, param_names)
% ```
%
% **Description**
%
% This function generates a series of jobs to run on the available cluster resources.
% `params` should be an M x N numerical matrix, where M is the number of parameters,
% and N is the number of simulations.
% `param_names` should be an M x 1 cell array of character vectors specifying xolotl properties.
%
% Jobs are apportioned between the available cluster resources.
%
% **Technical Details**
%
% xgrid interprets the `param_names` as the argument to the `x.get` function,
% For example, to get all maximal conductances, use
% ```matlab
% 	param_names = {'*gbar'};
% ```
%
% See Also:
% xgrid.cleanup
% xgrid.simulate
% xolotl.find

function batchify(self,params,param_names)
	corelib.assert(~isempty(self.x),'First configure the xolotl object')
	corelib.assert(~isempty(self.clusters),'At least one cluster has to be connected')

	% check that param names resolve correctly -- we do
	% so by attempting to read this property for each param name
	for i = 1:length(param_names)

		temp = self.x.get(param_names{i});
		corelib.assert(~isempty(temp),'Parameter names did not resolve correctly')
	end

	n_sims = size(params,2);
	self.n_sims = n_sims;
	corelib.assert(size(params,1) == length(param_names),'Param names does not match parameter dimensions')
	xhash = self.xolotl_hash;
	total_workers = sum([self.clusters.nthreads]);
	n_jobs = total_workers*self.n_batches;
	job_size = ceil(n_sims/n_jobs);
	idx = 1; c = 1;




	while idx <= n_sims
		z = idx+job_size-1;
		if z > n_sims
			z = n_sims;
		end
		param_idx = 1:n_sims;
		param_idx = param_idx(idx:z);
		this_params = params(:,idx:z);
		save([self.xgrid_folder filesep 'do' filesep 'job_' strlib.oval(c) '.ppp'],'this_params','param_names','xhash','param_idx');
		idx = z + 1; c = c + 1;
	end

	% now copy some of these files onto the remotes, if they exist
	do_folder = [self.xgrid_folder filesep 'do' filesep ];
	allfiles = dir([do_folder '*.ppp']);
	n_total_jobs = length(allfiles);

	job_distribution = ceil(([self.clusters.nthreads]./total_workers)*n_total_jobs);

	% move jobs onto clusters
	for i = 1:length(self.clusters)
		if strcmp(self.clusters(i).Name,'local')
			continue
		end
		corelib.verb(self.verbosity, 'INFO', ['Copying job files onto ' self.clusters(i).Name])


		% make a list of files to copy
		allfiles = dir([do_folder '*.ppp']);
		z = min([length(allfiles) job_distribution(i)]);
		files_to_copy = allfiles(1:z);

		% move them to a temp directory
		mkdir([do_folder 'remote_jobs'])
		for j = 1:length(files_to_copy)
			movefile([do_folder files_to_copy(j).name],[do_folder 'remote_jobs/' files_to_copy(j).name])
		end

		files_to_copy = dir([do_folder 'remote_jobs/*.ppp']);
		netlib.copyFiles(files_to_copy,[self.clusters(i).Name ':~/.psych/do/']);

		% delete files we just copied over
		for j = 1:length(files_to_copy)
			delete([files_to_copy(j).folder filesep files_to_copy(j).name])
		end

		% delete the entire temp folder
		rmdir([do_folder 'remote_jobs'])


	end

end % end batchify
