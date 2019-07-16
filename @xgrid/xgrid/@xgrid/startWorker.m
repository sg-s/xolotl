%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### startWorker
%
%
% **Syntax**
%
% ```matlab
% 	p.startWorker()
% ```
%
% **Description**
%
% SStarts a new worker.
% You can use this if you want to start up one worker at a time, for whatever reason.
%
% See Also:
% xgrid.addCluster

function startWorker(self)

% figure out how many workers there are already
job_idx = length(self.workers)+1;


F = parfeval(@self.simulate_core,0,job_idx,Inf);

self.workers = [self.workers; F];
