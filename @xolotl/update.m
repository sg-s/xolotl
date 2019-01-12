%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### update

**Syntax**


```matlab
xolotl.update()
```


**Description**

A static method that updates your installation of 
`xolotl` in place. If you installed using git, 
`xolotl` will attempt to use git to update itself.



!!! info "See Also"
    ->xolotl.uninstall

%}

function update()

% check if we are in a git repo 



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
	% no .git. maybe toolbox?
	toolboxes = matlab.addons.toolbox.installedToolboxes;
	if any(strcmp({toolboxes.Name},'xolotl'))
		% xolotl installed as a toolbox
		xolotl.uninstall();

		% download the new toolbox 
		websave('xolotl.mltbx','https://github.com/sg-s/xolotl/releases/download/latest/xolotl.mltbx');

		assert(exist('xolotl.mltbx') == 2,'Failed to download toolbox')

		t = matlab.addons.toolbox.installToolbox('xolotl.mltbx');

		disp('Installed xolotl version:')
		disp(t.Version)


	else
		error('Xolotl is not in a git repo, nor is it in a toolbox. Cannot update')
	end
end

