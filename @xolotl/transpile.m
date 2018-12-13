%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

# transpile


Generate a C++ file that constructs the model, 
integrates it, and moves parameters and data from 
MATLAB to C++ and back. 


Usage:


```
x.transpile;
```

!!! warning 
    Manually transpiling is discouraged. xolotl will automatically transpile code for you when needed. 
	

### See Also

xolotl.compile
xolotl.viewCode


%}


function transpile(self)

h = self.hash;
out_file = ['X_' h '.cpp'];
self.transpileCore('mexTemplate.cpp',out_file)


