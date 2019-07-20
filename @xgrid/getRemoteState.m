%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### getRemoteState
%
%
% **Syntax**
%
% ```matlab
% p.getRemoteState(idx)
% ```
%
% **Description**
%
% Fetches the state of a remote cluster by reading the log file.
% `idx` is the index of the cluster in `p.clusters`.
%
% **Technical Details**
%
% This function is *internal*.
%
% See Also:
% xgrid.showWorkerStates


function getRemoteState(self,idx)


disp('Getting log from remote...')
[e,~] = system(['scp ' self.clusters(idx).Name ':~/.psych/log.mat ' self.xgrid_folder '/' self.clusters(idx).Name '.log.mat']);
