%                 _     _ 
% __  ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/              
% 
%
% a MATLAB class to run parameter scans of neuron models
% runs using xolotl (https://github.com/sg-s/xolotl)
% needs the parallel computing toolbox

classdef xgrid < handle & matlab.mixin.CustomDisplay

	properties
		x@xolotl
		sim_func@function_handle
		% n_func_outputs % how many outputs will the simulation function generate?
		n_batches = 1 % per worker
		verbosity = 1;

		clusters

		stagger_time = 1 % seconds

		num_workers

		

	end % end props

	properties (SetAccess = protected)
		% allowed_param_names

		workers
		n_sims
		xolotl_hash
		current_pool@parallel.Pool
		daemon_handle@timer
		% controller_handle

		is_master = false;
		speed

	end

	properties (Access = protected)
		xgrid_folder
		sim_start_time
		n_outputs
	end


	methods (Access = protected)


        function displayScalarObject(self)
            url = 'https://github.com/sg-s/xgrid/';
            fprintf(['\b\b\b\b\b\b<a href="' url '">xgrid</a> '])
            if isempty(self.clusters)
            	 fprintf('is not connected to any cluster!')
            else
            	for i = 1:length(self.clusters)
            		fprintf(['\nis using ' strlib.oval(self.clusters(i).nthreads) ' threads on ' self.clusters(i).Name]);
            	end
            end

			fprintf('\n\nCluster      Delay   Queued  Running  Done  xolotl#\n')
			fprintf('---------------------------------------------------------------\n')
			for i = length(self.clusters):-1:1
				if strcmp(self.clusters(i).Name,'local')
					[n_do, n_doing, n_done] = getJobStatus(self);
					cluster_name_disp = strlib.fix('local',12);
					xhash = self.xolotl_hash;
					status = '';
				else


					if ~isfield(self.clusters(i),'plog')
						return
					end

					cluster_name_disp = self.clusters(i).Name;
					cluster_name_disp = strlib.fix(cluster_name_disp,12);
					n_do = self.clusters(i).plog.n_do; n_doing = self.clusters(i).plog.n_doing; n_done = self.clusters(i).plog.n_done;
					xhash = self.clusters(i).plog.xolotl_hash;
					% check that the log isn't stale
					if etime(datevec(now),datevec(self.clusters(i).plog.last_updated)) < 10
						status = [strlib.oval(etime(datevec(now),datevec(self.clusters(i).plog.last_updated))) 's'];
					else
						status = 'STALE';
					end

					% copy worker info onto local structure
					self.clusters(i).workers = [];
					if isfield(self.clusters(i).plog,'worker_diary')
						for j = 1:length(self.clusters(i).plog.worker_diary)
							self.clusters(i).workers(j).Diary = self.clusters(i).plog.worker_diary{j};
							self.clusters(i).workers(j).State = self.clusters(i).plog.worker_state{j};
						end
					end
				end
				if isempty(xhash)
					xhash = 'n/a         ';
				end
				fprintf([cluster_name_disp  ' ' strlib.fix(status,7) ' ' strlib.fix(strlib.oval(n_do),7) ' ' strlib.fix(strlib.oval(n_doing),8) ' ' strlib.fix(strlib.oval(n_done),5) ' ' xhash(1:7) '\n'])

			end

            % display the state of all the workers, on all nodes
            url = ['matlab:' inputname(1) '.showWorkerStates'];
            fprintf(['\n<a href="' url '">show worker states</a> \n'])



        end % end displayScalarObject
   end % end protected methods

	methods


		function self = xgrid(varargin)


			if ispc
				error('xgrid cannot run on a Windows computer')
			else
				self.xgrid_folder = '~/.psych';
				if exist(self.xgrid_folder,'file') == 7
				else
					mkdir(self.xgrid_folder)
				end
			end

			% wipe all old C++ files in .psych
			[e,o] = system('rm ~/.psych/*.hpp');

			% create do, doing, done folders if they don't exist
			if exist(pathlib.join(self.xgrid_folder,'do'),'file') == 7
			else
				mkdir(pathlib.join(self.xgrid_folder,'do'))
			end
			if exist(pathlib.join(self.xgrid_folder,'doing'),'file') == 7
			else
				mkdir(pathlib.join(self.xgrid_folder,'doing'))
			end
			if exist(pathlib.join(self.xgrid_folder,'done'),'file') == 7
			else
				mkdir(pathlib.join(self.xgrid_folder,'done'))
			end

			if nargin == 0
				self.addCluster('local')
				return
			end

			self.is_master = true;

			for i = 1:length(varargin)
				self.addCluster(varargin{i});
			end


		end


		function self = set.sim_func(self,value)

			% check if it exists
			corelib.assert(isa(value,'function_handle'),'Sim func must be a function handle')

			fs = func2str(value);
			loc = which(fs);
			corelib.assert(~isempty(loc),'Sim func could not be located')


			self.sim_func = value;

			% if there are remotes, copy this function onto the remotes, and ask them to configure it
			for i = 1:length(self.clusters)
				if strcmp(self.clusters(i).Name,'local')
					continue
				end

				% copy the sim function onto the remote
				netlib.copyFun(value,[self.clusters(i).Name ':~/.psych/']);


				% tell the remote to use this
				command = ['sim_func = @' func2str(value) ';'];
				self.tellRemote(self.clusters(i).Name,command);
			end
		end


		function self = set.x(self,value)


			corelib.assert(length(value)==1,'Only one xolotl object can be linked')
			self.x = value;

			if ~self.is_master & ~isempty(self.daemon_handle)
				% xgrid is being controlled
				% by a remote
				% assume that C++ header files exist
				% in ~/.psych/, and force cpplab to
				% only use those files
				cpplab.rebuildCache({'~/.psych/'})
				self.x.rebase;
			end


			try
				self.x.transpile;
				self.x.compile;
			catch err
				for i = 1:length(err.stack)
					disp(err.stack(i))
				end
			end

			self.xolotl_hash = self.x.hash;

			% also configure xolotl objects of all remotes
			for i = 1:length(self.clusters)
				if strcmp(self.clusters(i).Name,'local')
					continue
				end

				% copy over all C++ files onto the remote
				copy_these = self.x.generateHeaders;
				fprintf(['Copying C++ files to ' self.clusters(i).Name ' ...'])
				for j = 2:length(copy_these)
					if isempty(copy_these{j})
						continue
					end
					[e,~]=system(['scp ' copy_these{j} ' ' self.clusters(i).Name ':~/.psych/']);
					corelib.assert(e == 0,['Error while copying ' copy_these{j}])
				end
				fprintf('DONE\n')
				command = 'x = value';
				self.tellRemote(self.clusters(i).Name,command,value);
			end

		end % end set xolotl object



	end % end methods

	methods (Static)

		% this static method is to go from a voltage and calcium trace to burst metrics
		% assuming you have the calcium trace (true in simulations)
		[burst_metrics, spike_times, Ca_peaks, Ca_mins] = findBurstMetrics(V,Ca,varargin)
		spiketimes = findNSpikes(V,n_spikes,on_off_thresh)

		[neuron_metrics, phase_differences] = spiketimes2stats(varargin);




	end % end static methods

end % end classdef
