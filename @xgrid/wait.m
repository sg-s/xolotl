%                          _                                       
%                         | |                                      
%     _ __  ___ _   _  ___| |__   ___  _ __   ___  _ __ ___  _ __  
%    | '_ \/ __| | | |/ __| '_ \ / _ \| '_ \ / _ \| '_ ` _ \| '_ \ 
%    | |_) \__ \ |_| | (__| | | | (_) | |_) | (_) | | | | | | |_) |
%    | .__/|___/\__, |\___|_| |_|\___/| .__/ \___/|_| |_| |_| .__/ 
%    | |         __/ |                | |                   | |    
%    |_|        |___/                 |_|                   |_|
%  
% waits fpr all simulations to end on all clusters
function wait(self)

	keep_waiting = true;
	while keep_waiting
		keep_waiting = false;
		for i = 1:length(self.clusters)
			if strcmp(self.clusters(i).Name,'local')
				[n_do, n_doing] = self.getJobStatus
			else
				plog = self.getRemoteState(i);
				n_do = plog.n_do;
				n_doing = plog.n_doing;
			end


			if n_do > 0 | n_doing > 0
				pause(5)
				keep_waiting = true;
			end
		end
	end
end