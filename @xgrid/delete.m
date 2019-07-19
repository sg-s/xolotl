%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### delete
%
%
% **Syntax**
%
% ```matlab
% 	p.delete()
% ```
%
% **Description**
%
% Tries to stop the running daemons and removes the handle from the xgrid object.
%
%
% See Also:
% xgrid.stopDaemon

function delete(self)
try
	self.stopDaemon;
catch
end
