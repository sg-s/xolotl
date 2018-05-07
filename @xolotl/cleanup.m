%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: delete all mexBridge files and binaries

function cleanup()

this_dir = fileparts(fileparts(which('xolotl.cleanup')));

warning('Deleting compiled binaries...')
allfiles = dir([this_dir filesep '*mexBridge*']);
for i = 1:length(allfiles)
	delete([this_dir filesep allfiles(i).name]);
end
