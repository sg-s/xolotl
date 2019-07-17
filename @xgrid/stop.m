%                          _                                       
%                         | |                                      
%     _ __  ___ _   _  ___| |__   ___  _ __   ___  _ __ ___  _ __  
%    | '_ \/ __| | | |/ __| '_ \ / _ \| '_ \ / _ \| '_ ` _ \| '_ \ 
%    | |_) \__ \ |_| | (__| | | | (_) | |_) | (_) | | | | | | |_) |
%    | .__/|___/\__, |\___|_| |_|\___/| .__/ \___/|_| |_| |_| .__/ 
%    | |         __/ |                | |                   | |    
%    |_|        |___/                 |_|                   |_|
% 
% stop
% stops all simulations
% does not stop the daemon

function stop(self)
% stop workers on local machine
if ~isempty(self.workers)
	disp('Stopping all workers...')
	try
		cancel(self.workers)
	catch
	end
end

% move doing jobs back to queue
do_folder = [self.xgrid_folder filesep 'do' filesep ];
doing_folder = [self.xgrid_folder filesep 'doing' filesep ];

allfiles = dir([doing_folder '*.ppp']);
for i = 1:length(allfiles)
	this_job = allfiles(i).name;
	movefile([doing_folder this_job],[do_folder this_job])
end

% try to stop workers on remote clusters
for i = 1:length(self.clusters)
	if ~strcmp(self.clusters(i).Name,'local')
		self.tellRemote(self.clusters(i).Name,'stop;')
	end
end


% destroy all workers
self.workers = [];