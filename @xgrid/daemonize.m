%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### daemonize
%
%
% **Syntax**
%
% ```matlab
% 	p.daemonize()
% ```
%
% **Description**
%
% Sets up a daemon that listens for commands from xgrid.
%
%
% See Also:
% xgrid.addCluster
% xgrid.stopDaemon

function daemonize(self)


% stop all existing timers
t = timerfindall;
for i = 1:length(t)
	if any(strfind(func2str(t(i).TimerFcn),'xgrid'))
		stop(t(i))
		delete(t(i))
	end
end


% add the ~/.psych folder to the path so that sim functions can be resolved
addpath('~/.psych')


self.daemon_handle = timer('TimerFcn',@self.xgridd,'ExecutionMode','fixedDelay','TasksToExecute',Inf,'Period',1);
start(self.daemon_handle);

% make sure the parpool never shuts down
try
	pool = gcp('nocreate');
	pool.IdleTimeout = Inf;
catch
end
