%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### showWorkerStates
%
%
% **Syntax**
%
% ```matlab
% 	p.showWorkerStates()
% ```
%
% **Description**
%
% Prints the state of all workers on all clusters.
% Determines the state by reading the log files.
%
% **Technical Details**
%
% This function is *internal*.
%
% See Also:
% xgrid.simulate

function showWorkerStates(self)

fprintf('\n\nCluster      Worker  State      Output\n')
fprintf('---------------------------------------------------------------\n')

for i = 1:length(self.clusters)
	if strcmp(self.clusters(i).Name,'local')
		for j = 1:length(self.workers)
			cluster_name = strlib.fix('local',12);
			wid = strlib.fix(strlib.oval(j),7);
			ws = strlib.fix(self.workers(j).State,10);
			wd = self.workers(j).Diary;
			if ~isempty(wd)
    			try
        			wd = splitlines(wd);
            		if isempty(wd{end})
            			wd(end) = [];
            		end

    				wd = strlib.fix(wd{end},20);
    			catch
    				wd = strlib.fix('error parsing diary',20);
    			end
    		else
    			wd = strlib.fix('',20);
    		end
			fprintf([cluster_name  ' ' wid ' ' ws ' ' wd  '\n'])
		end
	else
		if isfield(self.clusters(i).plog,'worker_diary')
    		for j = 1:length(self.clusters(i).plog.worker_diary)
    			cluster_name = strlib.fix(self.clusters(i).Name,12);
    			wid = strlib.fix(strlib.oval(j),7);
    			ws = strlib.fix(self.clusters(i).plog.worker_state{j},10);
    			wd = self.clusters(i).plog.worker_diary{j};
    			if ~isempty(wd)
        			try
            			wd = splitlines(wd);
	            		if isempty(wd{end})
	            			wd(end) = [];
	            		end

        				wd = strlib.fix(wd{end},20);
        			catch

        				wd = strlib.fix('error parsing diary',20);
        			end
        		else
        			wd = strlib.fix('',20);
        		end
    			fprintf([cluster_name  ' ' wid ' ' ws ' ' wd  '\n'])
    		end
    	end
	end
end
