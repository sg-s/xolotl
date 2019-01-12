%{
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|


### checkCompartmentName

**Syntax**

```matlab
ok = checkCompartmentName(self,comp_name)
```

**Description**

`checkCompartmentName` is used internally by `xolotl` 
to verify that the compartment name you are using is valid and legal.  This method is called every time you add a compartment
to a `xolotl` object. 

!!! warning 
    Do not use `checkCompartmentName`, as it may be removed in a future release.

!!! info "See Also"
    ->xolotl.add

%}

function ok = checkCompartmentName(self,comp_name)

ok = false;

% check that the name doesn't conflict with the list of illegal names
if any(strcmp(comp_name,self.illegal_names))
	return
end

% no underscores allowed 
if strfind(comp_name,'_')
	return
end

ok = true;

