
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


if ~isempty(getCurrentTask)
	error('Compile was called on a parallel worker, which is not allowed. To avoid seeing this message, first run "integrate" on your model outside a parallel loop.')
end

h = self.hash;

mexBridge_name = [fullfile(filelib.cachePath('xolotl'),'X_') h '.cpp'];
corelib.assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')

if rem(self.verbosity,2) == 0
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

if rem(self.verbosity,3) == 0
	mex('-v',ipath,mexBridge_name,'-outdir',filelib.cachePath('xolotl'))
else
	mex('-silent',ipath,mexBridge_name,'-outdir',filelib.cachePath('xolotl'))
end

if isunix && ~ismac
	warning('on','MATLAB:mex:GccVersion');
end



% update linked_binary
self.linked_binary = ['X_' h '.' mexext];

if rem(self.verbosity,2) == 0
	disp(['[INFO] compilation successful!'])
end
