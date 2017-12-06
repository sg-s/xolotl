%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: delete all mexBridge files and binaries

function cleanup(self)

	f = fileparts(fileparts(which(mfilename)));

	allfiles = dir([f oss '*mexBridge*']);
	for i = 1:length(allfiles)
		delete([f oss allfiles(i).name]);
	end
end