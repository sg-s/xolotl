%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### xgridd
%
%
% **Syntax**
%
% ```matlab
% 	p.xgridd(~, ~)
% ```
%
% **Description**
%
% This is the daemon version of xgrid.
% It's a very simple loop that is meant to be run on a timer.
% Every time it runs, it looks to see if there is a command that tells it to do something,
% and if so, tries to do it.
%
% **Technical Details**
%
% This function should *never* throw an error,
% so count on it to be running at all times.
%

function xgridd(self,~,~)


% check if there is a xgrid object in the workspace,
% otherwise kill itself

vars = evalin('base','whos');
if ~any(strcmp({vars.class},'xgrid'))
	d = dbstack;
	if any(strcmp({d.name},'xgrid.xgrid'))
		return
	end
	disp('Stopping daemon...')
	stop(self.daemon_handle)
	delete(self.daemon_handle)
	return
end



if self.is_master
	% this is run only if it's a controller
	for i = 1:length(self.clusters)
		if strcmp(self.clusters(i).Name,'local')
			continue
		end
		parfeval(@self.getRemoteState,0,i);

		% load the log


		try
			load([self.xgrid_folder '/' self.clusters(i).Name '.log.mat']);
			self.clusters(i).plog = plog;
		catch
		end

	end

else

	% running as slave
	% always print a log
	self.printLog;


	% run any commands specified by master
	response = 0;

	if exist('~/.psych/com.mat','file') == 2

		% wipe any old responses if they exist
		if exist('~/.psych/com_response.mat','file') == 2
			delete('~/.psych/com_response.mat')
		end

		try
			load('~/.psych/com.mat')
			delete('~/.psych/com.mat')
			disp(['Running command ' command])
		catch err
			disp(err)
			response = 1;
			save('~/.psych/com_response.mat','response')
		end

		try
			eval(['self.' command])
			disp('Command completely successfully!')
			save('~/.psych/com_response.mat','response')

		catch err
			disp(err)
			response = 1;
			save('~/.psych/com_response.mat','response')
		end

	end

end
