%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### setup

**Syntax**

```matlab
xolotl.setup
x.setup
```

**Description**

A static method that allows you to set up compilers 
on some operating systems. You need to run this only 
once. If xolotl works, there is no need to run this. 


!!! info "See Also"
    ->xolotl.update
    ->xolotl.uninstall

%}


function setup()


if ispc

	error('Automatic setup not possible on Windows. You will have to download and isntall the mingw compiler manually (good luck!).')

elseif ismac

	mex -setup C
	mex -setup C++

else 

	% guess if this is Ubuntu
	[e,o] = system('lsb_release -a');
	assert(e ==0,'Unknown OS. Cannot automatically setup')

	if any(strfind(o,'Ubuntu'))

		!sudo apt-get update
		!sudo apt-get install g++ -y
		!sudo apt-get install gcc -y
		mex -setup C
		mex -setup C++

	else
		error('Unknown OS. Cannot automatically setup')
	end


end

