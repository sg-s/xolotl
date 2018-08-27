%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

update
^^^^^^
A static method that updates your installation of xolotl in place. If you installed using git, ``xolotl`` will attempt to use git to update itself. Usage ::

   xolotl.update
   x.update

%}

function update()

% check if we are in a git repo 

if ispc
	warning('Updating not supported on Windows')
	return

end

if exist([fileparts(fileparts(which('xolotl'))) filesep '.git'],'dir') == 7
	% .git exists

	% is git installed?
	 [~,o]=system('git');
	 if any(strfind(o,'checkout'))
	 	% git is installed
	 	% attempt to pull
	 	!git stash
	 	!git pull
	 else
	 	error('.git folder exists, but git does not. cannot update.')
	 end


else
	% no .git
	warning('not a git repo, will not update.')
end

