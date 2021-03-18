
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### upload
%
%
% **Syntax**
%
% ```matlab
% x.upload;
% ```
%
% **Description**
%
% Uploads a compiled binary to Github so that 
% others can use it. For this to work, you will
% need: 
% 
% 1. write permissions to github.com/sg-s/xolotl
% 2. The github CLI (gh) installed and configured
%
% This method will fail silently without any 
% log, and is not meant for public use.    
%


function upload(self)


binary_loc = fullfile(filelib.cachePath('xolotl'),['X_' self.hash '.' mexext]);

try

	self.integrate;

	[e,o] = system(['/usr/local/bin/gh release upload bin ' binary_loc ' --repo sg-s/xolotl']);

	if any(strfind(o,'Successfully uploaded'))
		disp(['Uploaded binary with hash: ' self.hash])
	end

	if any(strfind(o,'asset under the same name already exists'))
		disp('This binary already exists on Github')
	end

catch

end