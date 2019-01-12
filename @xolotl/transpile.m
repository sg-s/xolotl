%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### transpile


**Syntax**

```matlab
x.transpile;
```

**Description**

Generate a C++ file that constructs the model, 
integrates it, and moves parameters and data from 
MATLAB to C++ and back. 



!!! warning 
    Manual transpiling is discouraged. xolotl will automatically transpile code for you when needed. 
	

!!! info "See Also"
   ->xolotl.compile
   ->xolotl.viewCode


%}


function transpile(self)

self.checkTree;

h = self.hash;
out_file = ['X_' h '.cpp'];
self.transpileCore('mexTemplate.cpp',out_file)


