%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### uninstall

**Syntax**

```matlab
xolotl.uninstall
x.uninstall
```


**Description**

A static method that uninstalls your installation 
of xolotl in place. If you installed using git, 
`xolotl` will attempt to use git to uninstall 
itself. 



!!! info "See Also"
    ->xolotl.update

%}

function uninstall()

toolboxes = matlab.addons.toolbox.installedToolboxes;

% go somewhere safe
if ispc
	gohere = winqueryreg('HKEY_CURRENT_USER',['Software\Microsoft\Windows\CurrentVersion\','Explorer\Shell Folders'],'Personal');
else
	gohere = '~';
end
cd(gohere)

% remove all toolboxes with "xolotl" in it
for i = 1:length(toolboxes)
	if strcmp(toolboxes(i).Name,'xolotl')
		matlab.addons.toolbox.uninstallToolbox(toolboxes(i));
	end
end