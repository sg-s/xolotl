%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### go_to_examples


**Syntax**

```matlab
xolotl.go_to_examples
```

**Description**

A static method that goes to the folder that contains xolotl examples. 

!!! info "See Also"
    ->xolotl.run_all_tests

%}


function go_to_examples()

cd([fileparts(fileparts(which('xolotl'))) filesep 'examples'])	