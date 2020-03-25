### Something went wrong; what are my first steps?

In MATLAB, try running:

```matlab
xolotl.cleanup
cpplab.rebuildCache
```

These two functions will remove old compiled C++ binaries
and check to make sure that xolotl can find the C++ files
that define compartments and conductances and so on.


### On macOS, I get an error saying "xcodebuild: error: SDK "macosx10.15.4" cannot be located."


The full error looks like:

```
Error using mex
xcodebuild: error: SDK "macosx10.15.4" cannot be located.
xcrun: error: sh -c
'/Applications/Xcode.app/Contents/Developer/usr/bin/xcodebuild -sdk
macosx10.15.4 -find clang++ 2> /dev/null' failed with exit code 16384: (null)
(errno=No such file or directory)
xcrun: error: unable to find utility "clang++", not a developer tool or in PATH
```

The problem here is that Apple, in its infinite wisdom, has silently updated XCode, and has changed some defaults, which MATLAB blindly reads. Unfortunately, the defaults are wrong, and won't work. 

Here's how to diagnose and fix the problem.

First, check that you can reproduce the same error in your terminal. Paste this command in your terminal (not your MATLAB command prompt) and check that you get the same error:

```bash
/usr/bin/xcrun -sdk macosx10.15.4 clang++
```

If so, then the problem is with the `-sdk` flag. By trial and error, I figured out that this worked:

```bash
/usr/bin/xcrun -sdk macosx10.15 clang++
```

The value that works for you may be different. Once you figure out what works, we need to tell MATLAB to use this flag.

Use 

```matlab
m = mex.getCompilerConfigurations('C++')
```

to see what MATLAB knows about C++ compilers and linkers.

You should see something like this:

```
  CompilerConfiguration with properties:

             Name: 'Xcode Clang++'
     Manufacturer: 'Apple'
         Language: 'C++'
          Version: ''
         Location: '/Applications/Xcode.app/Contents/Developer'
        ShortName: 'Clang++'
         Priority: 'A'
          Details: [1×1 mex.CompilerConfigurationDetails]
       LinkerName: '/usr/bin/xcrun -sdk macosx10.15 clang++'
    LinkerVersion: ''
           MexOpt: '/Users/srinivas/Library/Application Support/MathWorks/MATLAB/R2018a/mex_C++_maci64.xml'
```

The crucial line is the `LinkerName` property, where we see that it has the wrong `-sdk` flag. To change this, edit the xml file in the location indicated in the `MexOpt` field, and replace all instances of `macosxXXXXX` with `macosx10.15` or whatever works on your computer.


Then, running `mex -setup C++` and `m = mex.getCompilerConfigurations('C++')` should show the updated ` LinkerName` and things should work again.

### On macOS, I get an annoying warning saying "xcrun: error: SDK "macosx10.13.4" cannot be located"

Run the following in your shell (not the MATLAB prompt):

```bash
sudo xcode-select -s /Applications/Xcode.app
```

### On macOS, I get a warning saying that "Warning: Xcode is installed, but its license has not been accepted."

* First, make sure you have [XCode](https://developer.apple.com/xcode/) installed (not just the Command Line Tools -- the whole thing).
* You can get this from the Mac App Store.
* Then, open XCode and accept the license.
* You will have to do this only once.


### When trying to run code on parallel, I get an error saying "File not found!" or "Error in xolotl/transpile (line 43) self.transpileCore('mexTemplate.cpp',out_file)"

You're getting this error because one or more of MATLAB's parallel processes can't find the mexTemplate.cpp file that it needs to compile code. This is because even though xolotl is added on your path, it isn't on your path in *all* copies of the main MATLAB process that are running in parallel. 

To fix this, save your path and reboot your parallel workers:

```matlab
savepath; % saves path
delete(gcp) % delete the current parallel pool
rehash toolbox
rehash toolboxcache % updates caches
parpool % restart parallel pool

```

### I ran the quickstart, but I don't see anything

Are you using a tiny screen? Some UI elements may go out of the frame on very small screens. To fix this, acquire the handle to the figure and change the position property. For example:

```
x.manipulate;
manip = gcf;
manip.Position = [0 0 300 100];
```

### I get an error saying I don't have a compiler

You need a C/C++ compiler. You need to follow MATLAB's
[instructions](https://www.mathworks.com/support/compilers.html)
on how to get one, how to install one, and how to configure one.
It may be helpful to also see our advice on [compilers](how-to/install-configure.md)

### I created a new component, but `cpplab` can't find it

This typically appears as a `cpplab::resolvePath` error. Here are some things to check:

* Are you sure that your named it `*****.hpp` and not `*****.cpp`?
* Does it exist somewhere on your MATLAB path, or in a subfolder?
* Did you try running `cpplab.rebuildCache`?
* Did you try `x = xolotl; x.rebase`?
* If you still run into errors, file an issue at [cpplab](https://github.com/sg-s/cpplab/issues/)
