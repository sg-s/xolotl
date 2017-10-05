function viewCode(self)
	h = self.hash;
	h = h(1:6);
	c = ['mexBridge' h '.cpp'];
	c = joinPath(self.xolotl_folder,c);
	if exist(c,'file') == 2
		edit(c);
	else
		error('No C++ file matching this configuration. Use "transpile"')
	end
end
