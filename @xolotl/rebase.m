
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### rebase
%
% **Syntax**
%
% ```matlab
% x.rebase()
% ```
%
% **Description**
%
% `rebase` is an internal method that configures some
% house-keeping settings. rebase is called every
% time a new xolotl object is created. rebase:
%
% 1. configures the `xolotl_folder` property
% 2. configures the `cpp_folder` property, which tells xolotl where its C++ files are located
% 3. calls the rebase method from the cpplab superclass.
%
%
% If you move a xolotl object across computers (for example, by saving it to a file and loading it in a different computer), you must call `rebase` to link it to the C++ files it needs.
%
%
% See Also: 
% cpplab.rebase
% 




function rebase(self)

self.xolotl_folder = fileparts(fileparts(which('xolotl')));
self.cpp_folder = fullfile(self.xolotl_folder,'c++');

% update all C++ class paths
rebase@cpplab(self);


% check that we have a working compiler
compilerok = false;
try
	compilerok = getpref('xolotl','compilerok');
catch
end

if compilerok
	return
end

cc = mex.getCompilerConfigurations('C++');

if length(cc) > 0
	compilerok = true;
end

