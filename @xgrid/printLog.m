%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### printLog
%
%
% **Syntax**
%
% ```matlab
% 	p.printLog()
% ```
%
% **Description**
%
% Generates log files on each cluster.
% The file contains the job status and state of each worker.
%
% See Also:
% xgrid.getJobStatus
% xgrid.showWorkerStates


function printLog(self)

% delete old log files if any
if exist(pathlib.join(self.xgrid_folder,'log.mat'),'file')
	delete(pathlib.join(self.xgrid_folder,'log.mat'))
end

% start logging
plog.host_name = strtrim(corelib.getComputerName);
plog.nthreads = 2*feature('numcores');
plog.xolotl_hash = self.xolotl_hash;

[plog.n_do, plog.n_doing, plog.n_done] = self.getJobStatus;

plog.worker_diary = {};
plog.worker_state = {};

for i = 1:length(self.workers)
	plog.worker_diary{i} = self.workers(i).Diary;
	plog.worker_state{i} = self.workers(i).State;
end
plog.last_updated = now;

save(pathlib.join(self.xgrid_folder,'log.mat'),'plog')
