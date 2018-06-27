%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

cleanup
^^^^^^^
A static method that cleans up all transpiled ``C++`` and compiled binary files. Usage ::

   xolotl.cleanup
   x.cleanup

Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 

%}

function cleanup()

this_dir = fileparts(fileparts(which('xolotl.cleanup')));

warning('Deleting compiled binaries...')
allfiles = dir([this_dir filesep '*mexBridge*']);
for i = 1:length(allfiles)
	delete([this_dir filesep allfiles(i).name]);
end
