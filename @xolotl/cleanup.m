function cleanup(self)
	f = self.xolotl_folder;
	allfiles = dir([f oss '*mexBridge*']);
	for i = 1:length(allfiles)
		delete([f oss allfiles(i).name]);
	end
end