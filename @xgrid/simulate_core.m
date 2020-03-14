%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### simulate_core
%
%
% **Syntax**
%
% ```matlab
% 	p.simulate_core(idx, n_runs)
% ```
%
% **Description**
%
% Contains the main loop that performs a job during an xgrid simulation.
%
% **Technical Details**
%
% This function is *internal*.
% Users should call `simulate` instead.
%
% See Also:
% xgrid.simulate

function simulate_core(self,idx,n_runs)

	disp(['Worker ' mat2str(idx) ' booting up, waiting for start...'])
	pause(idx*self.stagger_time);
	disp(['Worker ' mat2str(idx) ' starting...'])

	while n_runs > 0

		% grab a job file and move it to doing
		do_folder = [self.xgrid_folder filesep 'do' filesep ];
		doing_folder = [self.xgrid_folder filesep 'doing' filesep ];
		done_folder = [self.xgrid_folder filesep 'done' filesep ];
		free_jobs = dir([ do_folder '*.ppp']);

		if isempty(free_jobs)
			return
		end

		try
			this_job = free_jobs(idx).name;
		catch
			this_job = free_jobs(1).name;
		end

		try
			movefile([do_folder this_job],[doing_folder this_job])
		catch
			pause(1)
			continue
		end

		% load the file
		load([doing_folder this_job],'-mat')


		% check that the hash matches
		corelib.assert(strcmp(xhash,self.xolotl_hash),'Hashes dont match')


		for i = 1:size(this_params,2)
			% update params

			self.x.set(param_names,this_params(:,i))

			% run the model
			sim_ok = false;


			try
				if isempty(self.n_outputs) || isnan(self.n_outputs)
					n_outputs = length(corelib.argOutNames(self.sim_func));
				else
					n_outputs = self.n_outputs;
				end
				[outputs{1:n_outputs}] = self.sim_func(self.x);
				sim_ok = true;
			catch err
				disp(err)
				for j = 1:length(err.stack)
					disp(err.stack(j))
				end
				warning('Error while running simulation function.')
			end

			% map the outputs to the data structures
			if sim_ok
				if ~exist('data','var')
					% create placeholders
					for j = length(outputs):-1:1
						data{j} = NaN(size(corelib.vectorise(outputs{j}),1),size(this_params,2));
					end
				end

				for j = 1:length(data)
					data{j}(:,i) = corelib.vectorise(outputs{j});
				end
			else err
				% write this error to disk
				save([hashlib.md5hash([doing_folder this_job]) '.error'],'err')
				warning('Something not OK, probably a bug in the simulation func')

				% move the job back to the queue and try again
				disp('Returning this job to the queue...')
				movefile([doing_folder this_job],[do_folder this_job])
				pause(1)

				break

			end

		end

		if ~sim_ok
			continue
		end

		% some defensive measures to make sure that data
		% and params are aligned
		ok = true;
		for j = 1:length(data)
			if size(data{j},2) ~= size(this_params,2)
				ok = false;
			end
		end

		if ok
			% all OK, can save the data, move on
			save([done_folder this_job '.data'],'data')

			% move the job into the done folder
			movefile([doing_folder this_job],[done_folder this_job])

		else
			% not OK. give up.
			disp('Something went wrong with this job:')
			disp(this_job)
			disp('This job will remain stuck in the doing queue')
		end



		clear data this_params param_idx param_names xhash
		n_runs = n_runs - 1;
	end

end
