%                 _     _ 
% __  ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/              
% 
% ### addCluster
%
% adds a compute cluster to the pool
%
% **Syntax**
% 
% ```matlab
% addCluster(self,cluster_name)
% ```
% 
% **Description**
% 
% Do not use this method. This method is called internally when
% you create a xgrid object. 
%
% See Also:
% xgrid.batchify
% xgrid.simulate
% xgrid.tellRemote

function addCluster(self,cluster_name)


if strcmp(cluster_name,'local')

	self.current_pool = gcp;
	self.num_workers = self.current_pool.NumWorkers - 1;

	if isempty(self.clusters)

		self.clusters(1).Name = cluster_name;
		self.clusters(1).nthreads = self.num_workers;
	else
		idx = length(self.clusters) + 1;
		self.clusters(idx).Name = cluster_name;
		self.clusters(idx).nthreads = self.num_workers;
	end


	% check if a benchmark exists
	try
		load('~/.psych/benchmark.mat','speed')
		self.speed = speed;
	catch
		disp('No benchmark data found. Running benchmarks. This will occur only once.')
		self.benchmark;
	end


else

	% check if we can ping the cluster
	fprintf(['\nPinging ' cluster_name '...'])

	[e,~]=system(['ping ' cluster_name ' -c 1']);


	assert(e == 0, 'Could not contact server -- check that you have the right name and that it is reachable')

	corelib.cprintf('Green','OK')


	% since this is being run as a controller, we'll need a daemon
	self.daemonize;
	
	stop(self.daemon_handle)
	self.daemon_handle.Period = 5;
	start(self.daemon_handle)

	% check we can SSH into the server, and that xgrid is running on that server

	fprintf('\nCopying log...')
	[e,~] = system(['scp ' cluster_name ':~/.psych/log.mat ' self.xgrid_folder '/' cluster_name '.log.mat']);

	if e == 0
		corelib.cprintf('Green','OK\n')
		% load the log 	
		try
			load([self.xgrid_folder '/' cluster_name '.log.mat']);
		catch
			pause(.5)
			load([self.xgrid_folder '/' cluster_name '.log.mat']);
		end
	else
		error('Could not connect to remote. error copying log from remote')
	end


	if isempty(self.clusters)
		self.clusters(1).Name = cluster_name;
		self.clusters(1).nthreads = plog.nthreads;

		try
			self.clusters(1).plog = plog;
		catch
		end

	else
		idx = length(self.clusters) + 1;
		self.clusters(idx).Name = cluster_name;
		self.clusters(idx).nthreads = plog.nthreads;

		try
			self.clusters(idx).plog = plog;
		catch
		end
	end




end
