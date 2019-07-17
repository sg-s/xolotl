%                          _                                       
%                         | |                                      
%     _ __  ___ _   _  ___| |__   ___  _ __   ___  _ __ ___  _ __  
%    | '_ \/ __| | | |/ __| '_ \ / _ \| '_ \ / _ \| '_ ` _ \| '_ \ 
%    | |_) \__ \ |_| | (__| | | | (_) | |_) | (_) | | | | | | |_) |
%    | .__/|___/\__, |\___|_| |_|\___/| .__/ \___/|_| |_| |_| .__/ 
%    | |         __/ |                | |                   | |    
%    |_|        |___/                 |_|                   |_|
%  
% return the status of jobs on local machine 

function [n_do, n_doing, n_done] = getJobStatus(self)
	do_folder = [self.xgrid_folder filesep 'do' filesep ];
	doing_folder = [self.xgrid_folder filesep 'doing' filesep ];
	done_folder = [self.xgrid_folder filesep 'done' filesep ];
	free_jobs = dir([ do_folder '*.ppp']);
	running_jobs = dir([ doing_folder '*.ppp']);
	done_jobs = dir([ done_folder '*.ppp']);
	n_do = length(free_jobs);
	n_doing = length(running_jobs);
	n_done = length(done_jobs);
end