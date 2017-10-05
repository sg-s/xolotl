
function [] = compile(self)
	h = self.hash;
	mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
	assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')
	mex('-silent',mexBridge_name,'-outdir',self.xolotl_folder)
	% update linked_binary

	self.linked_binary = [pathEnd(mexBridge_name) '.' self.OS_binary_ext];

end