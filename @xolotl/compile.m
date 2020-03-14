
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### compile
%
% **Syntax**
%
% ```matlab
% x.compile
% ```
%
% **Description**
%
% compiles a executable binary form a transpiled ``C++`` file.
% These are stored in your ``xolotl`` directory. ``xolotl``
% automatically compiles when t needs to. You can turn this
% functionality off by setting
%
%
% See Also: 
% xolotl.transpile
% xolotl.cleanup



function compile(self)

h = self.hash;

mexBridge_name = [pathlib.join(self.xolotl_folder,'X_') h '.cpp'];
corelib.assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')

if self.verbosity > 0
	disp(['[INFO] compiling using mex...'])
end

% tell mex about where to look for C++ files
if ispc
	ipath = ['-I' self.xolotl_folder '/c++/'];
else
	ipath = ['-I"' self.xolotl_folder '/c++/"'];
end

if isunix && ~ismac
	warning('off','MATLAB:mex:GccVersion');
end

if self.verbosity > 1
	mex('-v',ipath,mexBridge_name,'-outdir',self.xolotl_folder)
else
	mex('-silent',ipath,mexBridge_name,'-outdir',self.xolotl_folder)
end

if isunix && ~ismac
	warning('on','MATLAB:mex:GccVersion');
end



% update linked_binary
self.linked_binary = ['X_' h '.' mexext];

if self.verbosity > 0
	disp(['[INFO] compilation successful!'])
end
