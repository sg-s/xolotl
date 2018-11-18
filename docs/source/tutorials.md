# Tutorials 

## Getting started (Installation)

Let's install `xolotl`. You will need:

1. [MATLAB](https://www.mathworks.com/products/matlab.html) 	`xolotl` will not work on GNU Octave.
2. An internet connection

Download [xolotl here](https://github.com/sg-s/xolotl/releases/download/latest/xolotl.mltbx) and drag the downloaded file (`xolotl.mltbx`) onto your MATLAB workspace. MATLAB should automatically open a prompt asking you if you want to install it. 

Then we need to install a C/C++ compiler, and get MATLAB to work with that. Follow these instructions based on your operating system:

### macOS 

1. Install [XCode](https://developer.apple.com/xcode/)
2. Open XCode, and accept the license agreement.
3. Run this in your MATLAB prompt:

```
mex -setup c++
mex -setup c
```

### Windows

1. Download and install the [MinGW compiler](https://www.mathworks.com/matlabcentral/fileexchange/52848-matlab-support-for-mingw-w64-c-c-compiler) from the Mathworks FileExchange. You'll need to log in using a Mathworks account. This ridiculous restriction is Mathworks' fault. 
2. Run this in your MATLAB prompt:

```
mex -setup c++
mex -setup c
```

### GNU/Linux

1. Use your favorite package manager to install a C and C++ compiler. For example, on Debian-based systems, use `sudo apt install gcc & sudo apt install g++` 
2. Verify that your compiler is on the MATLAB path by running `system('which gcc')` from within MATLAB
3.  Run this in your MATLAB prompt:

```
mex -setup c++
mex -setup c
```

If you still have trouble setting up compilers ant MATLAB, see this section [INSERT LINK HERE]

## Create a single compartment bursting neuron

In this tutorial we will use a worked example to simulate a single compartment bursting neuron model. 

Run this code in your MATLAB prompt:

```
xolotl.go_to_examples
demo_bursting_neuron
```

You should see a plot appear showing the voltage trace of a bursting neuron. You can see what effect changing the maximal conductances has on the voltage dynamics by running:

```
x.manipulate('*gbar')
```

A GUI will appear with sliders for maximal conductances of all the channels in the model. Try moving them around and seeing what happens with the voltage dynamics. 

To see how this model was created, and to use this as a template for your own projects, type:

```
edit demo_bursting_neuron.m
```



## Simulate a voltage-clamp experiment


In this tutorial we will use a worked example to simulate a voltage-clamp experiment. 

Run this code in your MATLAB prompt:

```
xolotl.go_to_examples
demo_clamp
```
A figure should appear showing clamping currents for various voltages, together with a I-V curve for this model. 

You will notice that the xolotl object `x` has a field called `V_clamp`. Look inside that -- it's a long vector. Try setting it to a different value, for example:

```matlab
x.V_clamp = -10; % mV
```

and then integrate the model using:

```
I = x.integrate;
```

You can plot the current using:

```matlab
plot(I)
```

To see how this model was created, and to use this as a template for your own projects, type:

```
edit demo_clamp.m

```

