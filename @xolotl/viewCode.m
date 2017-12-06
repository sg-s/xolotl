%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: view the C++ mexBridge code in your default editor

function viewCode(self)
	shs = self.skip_hash_check;
	self.skip_hash_check = false;
	h = self.hash;
	self.skip_hash_check = shs;
	h = h(1:6);
	c = ['mexBridge' h '.cpp'];
	c = joinPath(self.xolotl_folder,c);
	if exist(c,'file') == 2
		edit(c);
	else
		error('No C++ file matching this configuration. Use "transpile"')
	end
end
