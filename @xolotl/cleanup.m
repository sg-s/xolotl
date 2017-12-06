%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: delete all mexBridge files and binaries

function cleanup(self)


	allfiles = dir([self.xolotl_folder oss '*mexBridge*']);
	for i = 1:length(allfiles)
		delete([self.xolotl_folder oss allfiles(i).name]);
	end
end