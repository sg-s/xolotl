
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% ### update
%
% **Syntax**
%
% ```matlab
% xolotl.update
% ```
%
% **Description**
%
% Static method of xolotl that updates the currently
% installed version of xolotl to the latest version.
% This method needs an active internet connection to 
% work. If xolotl was installed using git, it will
% attempt to use git to update. If xolotl was installed
% as a MATLAB toolbox, it will download the latest 
% version of the toolbox and will attempt to replace the
% installed version with the latest version.
% 
% !!! warning
% XXXXUpdating xolotl if installed as a MATLAB toolbox will overwrite preferences and delete compiled binaries. 
%
% See Also: 
% xolotl.uninstall
% 

function update()

update@UpdateableHandle('https://github.com/sg-s/xolotl','xolotl')