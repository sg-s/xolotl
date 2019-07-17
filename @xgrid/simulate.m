%                          _                                       
%                         | |                                      
%     _ __  ___ _   _  ___| |__   ___  _ __   ___  _ __ ___  _ __  
%    | '_ \/ __| | | |/ __| '_ \ / _ \| '_ \ / _ \| '_ ` _ \| '_ \ 
%    | |_) \__ \ |_| | (__| | | | (_) | |_) | (_) | | | | | | |_) |
%    | .__/|___/\__, |\___|_| |_|\___/| .__/ \___/|_| |_| |_| .__/ 
%    | |         __/ |                | |                   | |    
%    |_|        |___/                 |_|                   |_|
%  
% start the simulation on all cluster, local and remote

function simulate(self)

	assert(~isempty(self.sim_func),'No sim function configured, refusing to start')
    

	% make sure there exists a linked binary
	if isempty(self.x.linked_binary)
		self.x.skip_hash_check = false;
		self.x.transpile;
		self.x.compile;
		self.x.skip_hash_check = true;
	end

	for i = 1:length(self.clusters)
		if strcmp(self.clusters(i).Name,'local')

			self.sim_start_time = now;

			if self.verbosity
				disp('Starting workers...')
			end

			for j = 1:self.num_workers
				self.startWorker();
				corelib.textbar(j, self.num_workers)
			end


		else
			% it's a remote cluster. ask the remote (nicely)
			% to start the simulations

			command = ['simulate();'];
			self.tellRemote(self.clusters(i).Name,command);


		end
	end



end % end simulate 
