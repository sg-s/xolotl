
### On macOS, I get an annoying warning saying "xcrun: error: SDK "macosx10.13.4" cannot be located"

Run the following in your shell (not the MATLAB prompt):

```bash
sudo xcode-select -s /Applications/Xcode.app
```

### On macOS, I get a warning saying that "Warning: Xcode is installed, but its license has not been accepted."

First, make sure you have [XCode](https://developer.apple.com/xcode/) installed (not just the Command Line Tools -- the whole thing). You can get this from the Mac App Store. Then, open XCode and accept the license. You will have to do this only once.

### On Linux, I get a "This version of GCC is not supported" warning.

This is a common warning message. MATLAB only supports old versions of the gcc compiler,
though in most cases this doesn't prevent xolotl from functioning normally. There are
several ways to fix this warning. First, you can uninstall the version of gcc MATLAB doesn't
want and install the proper version, though if everything else is working normally, it's
probably best not to touch it. Better yet, you can try

```matlab
warning('off','MATLAB:mex:GccVersion');
```

which should disable the warning message.

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

* Are you sure that your named it `*****.hpp` and not `*****.cpp`?
* Does it exist somewhere on your MATLAB path, or in a subfolder?
* Did you try running `cpplab.rebuildCache`?
* If you still run into errors, file an issue at [cpplab](https://github.com/sg-s/cpplab/issues/)
