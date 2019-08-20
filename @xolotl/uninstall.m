
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% ### uninstall
%
% **Syntax**
%
% ```matlab
% xolotl.uninstall
% ```
%
% **Description**
%
% Static method of xolotl that uninstalls the currently
% installed version of xolotl. If xolotl was installed
% as a MATLAB toolbox, this method deletes the toolbox
% and removes the toolbox from the path.
%
% !!! warning
% XXXXCreating two snapshots with the same name will overwrite the first.
%
% See Also: 
% xolotl.update
% 



function uninstall()

uninstall@UpdateableHandle('xolotl')