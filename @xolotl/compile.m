%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: compiles a C++ file using mex
% 

function compile(self)

h = self.hash;

mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')

if self.debug_mode
	disp(['[INFO] compiling using mex...'])
end

% tell mex about where to look for C++ files
ipath = ['-I"' self.xolotl_folder '/c++/' '"'];
if self.debug_mode
	mex('-v',ipath,mexBridge_name,'-outdir',self.xolotl_folder)
else
	mex('-silent',ipath,mexBridge_name,'-outdir',self.xolotl_folder)
end



% update linked_binary
self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];

if self.debug_mode
	disp(['[INFO] compilation successful!'])
end