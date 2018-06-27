%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

compile
^^^^^^^

compiles a executable binary form a transpiled ``C++`` file. These are stored in your ``xolotl`` directory. ``xolotl`` automatically compiles when t needs to. You can turn this functionality off by setting ::

x.skip_hash = true;

In addition, creating a ``xolotl`` object through a function call does not utomatically hash and compile. In this case, you should use ``x.md5hash``.

.. warning::

If you turn hashing off, ``xolotl`` might not compile


See Also:
---------

- transpile
- cleanup

%}

function compile(self)

h = self.hash;

mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
assert(exist(mexBridge_name,'file')==2,'C++ file to compile does not exist. Use "transpile" before compiling')

if self.verbosity > 0
	disp(['[INFO] compiling using mex...'])
end

% tell mex about where to look for C++ files
if ispc
	ipath = ['-I' self.xolotl_folder '/c++/'];
else
	ipath = ['-I"' self.xolotl_folder '/c++/"'];
end
if self.verbosity > 1
	mex('-v',ipath,mexBridge_name,'-outdir',self.xolotl_folder)
else
	mex('-silent',ipath,mexBridge_name,'-outdir',self.xolotl_folder)
end



% update linked_binary
self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];

if self.verbosity > 0
	disp(['[INFO] compilation successful!'])
end