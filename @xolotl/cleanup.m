%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### cleanup

**Syntax**

```matlab
xolotl.cleanup
x.cleanup
```

**Description**

A static method that cleans up all transpiled ``C++`` and compiled binary files.

!!! warning 
    Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 

%}

function cleanup()

this_dir = fileparts(fileparts(which('xolotl.cleanup')));

d = dbstack;
if ~any(strcmp({d.name},'run_all_tests'))
	warning('[xolotl::cleanup] Deleting compiled binaries...')
end
allfiles = dir([this_dir filesep '*X_*']);
for i = 1:length(allfiles)
	delete([this_dir filesep allfiles(i).name]);
end
