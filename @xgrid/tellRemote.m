%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### tellRemote
%
%
% **Syntax**
%
% ```matlab
% p.tellRemote(cluster_name, command, value)
% ```
%
% **Description**
%
% Do not use this method. 
%
%
% See Also:
% xgrid.addCluster


function status = tellRemote(self,cluster_name,command,value)

if nargin < 4
	value = 0;
end

% delete any response on the local machine
if exist('~/.psych/com_response.mat') == 2
	delete('~/.psych/com_response.mat')
end

% delete responses on the remote
[e,o] = system(['ssh ' cluster_name ' " rm ~/.psych/com_response.mat " ']);

if e ~= 0
	if any(strfind(o,'No such file or directory'))
	else
		error('Error deleting old response on remote')
	end
end

save('~/.psych/com.mat','command','value');


[e,o] = system(['scp ~/.psych/com.mat ' cluster_name ':~/.psych/']);

if e ~=0
	disp(o)
	error('could not copy command to remote')
end
pause(1)
tic
goon = true;

if any(strfind(command,'simulate'))
	t_timeout = Inf;
else
	t_timeout = 10;
end

while goon
	[e,o] = system(['scp ' cluster_name ':~/.psych/com_response.mat ~/.psych/']);
	pause(1)

	if e == 0
		load('~/.psych/com_response.mat')
		if response == 0
			goon = false;
			return
		else
			error('Remote responded with an error.')
		end

	end

	t = toc;

	if t > t_timeout
		goon = false;
		error('Command timed out.')
	end

end
