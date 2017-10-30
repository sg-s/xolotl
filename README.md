[![Build Status](https://travis-ci.org/sg-s/xolotl.svg?branch=master)](https://travis-ci.org/sg-s/xolotl)

![](https://user-images.githubusercontent.com/6005346/30719217-a1e8d54a-9ef0-11e7-8889-c284ac13163e.png)

# xolotl: The Standard Network Simulator 

`xolotl` is a fast single-compartment and multi-compartment simulator written in C++, with MATLAB wrappers. `xolotl` is written with a focus on flexibility and speed. It can simulate single-compartment conductance-based neuron models, networks of these, and detailed multi-compartment models. Because it's written in C++, it's really fast (see [benchmarks](#benchmarks)). 

## Usage (C++)

The most basic way to use `xolotl` is from C++. 

Set up a simple single-compartment neuron:

```c++
// Set up some conductances 
NaV gna(1000,e_na, mNa, hNa);
Kd gkd(1000,e_k, mKd);
Leak gleak(1,e_leak);

// create a compartment 
compartment HH(-70, .01, Cm, A, f, Ca_out, Ca_in, tau_Ca);

// add the conductances to the compartment 
HH.addConductance(&gna);
HH.addConductance(&gkd);
HH.addConductance(&gleak);
```

Integrate this using a time step of `50 us`:

```c++
HH.integrate(50e-3);
```

`xolotl` also makes it easy to set up networks of neurons, or even multi-compartment neurons (the same code works for both). 

## Usage (MATLAB)

> Read the detailed documentation for MATLAB usage [here](docs/matlab/README.md)!

`xolotl.m` is a MATLAB class that generates and compiles C++ wrapper code on the fly, and uses that to run simulations, while presenting a pseudo-object-oriented interface within MATLAB. 

Set up a `xolotl` instance:

```matlab
x = xolotl;
```

Set up a basic neuron and add some conductances:

```matlab
x.addCompartment('AB',-60,0.02,10,0.0628,1,1443,3000,0.05,200,0);
x.addConductance('AB','liu/NaV',@() 115/x.AB.A,30);
x.addConductance('AB','liu/CaT',@() 1.44/x.AB.A,30);
x.addConductance('AB','liu/CaS',@() 1.7/x.AB.A,30);
x.addConductance('AB','liu/ACurrent',@() 15.45/x.AB.A,-80);
x.addConductance('AB','liu/KCa',@() 61.54/x.AB.A,-80);
x.addConductance('AB','liu/Kd',@() 38.31/x.AB.A,-80);
x.addConductance('AB','liu/HCurrent',@() .6343/x.AB.A,-20);
x.addConductance('AB','Leak',@() 0.0622/x.AB.A,-50);

```

Inspect the `xolotl` object you have configured:

```matlab
x = 

xolotl object with:
---------------------
+ AB  
  > NaV (g=115/x.AB.A, E=30)
  > CaT (g=1.44/x.AB.A, E=30)
  > CaS (g=1.7/x.AB.A, E=30)
  > ACurrent (g=15.45/x.AB.A, E=-80)
  > KCa (g=61.54/x.AB.A, E=-80)
  > Kd (g=38.31/x.AB.A, E=-80)
  > HCurrent (g=.6343/x.AB.A, E=-20)
  > Leak (g=0.0622/x.AB.A, E=-50)
---------------------

```

This tells you that there is one compartment (called `AB`), and that compartment has 8 types of channels in it. The maximum conductances and reversal potentials of each channel type are shown above. Note that the maximum conductances are functions of another parameter of the compartment, the area. Thus, changing the area of the `AB` compartment will also change the specific maxmimum conductances of all channels. 

You can drill as deep as you want into the structure, and modify values if you want to. In this example, we're setting the reversal potential of the `NaV` channel on neuron `AB` to 50mV;

```matlab
x.AB.NaV.E = 50; % mV
```


Integrate and plot the voltage:

```matlab
x.dt = 50e-3;
x.t_end = 5000;
[V,Ca] = x.integrate;
figure, plot(V)
```

![](https://user-images.githubusercontent.com/6005346/30713658-ff96faf4-9edd-11e7-9db1-a2ca4f2f0567.png)

Behind the scene, MATLAB [transpiles](https://en.wikipedia.org/wiki/Source-to-source_compiler) MATLAB code describing your network into C++, and compiles it using `mex` for your architecture. One of the advantages of the MATLAB interface is that you can manipulate all parameters of your neuron/network model, and see what the effect of that is live. This makes it much easier to get an intuition of how your model behaves. 

This is as simple as:

```matlab
x.manipulate;
```

![](https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif)

## Get this

### Requirements

| OS          | Support | Comments | 
| -------          | ------- | ----------- 
| macOS 10.12.6 | ✓ | no known issues |
| Ubuntu 16.04.3 LTS | ✓ |   `mex` complains about incompatible compilers, but everything works | 
| Windows | ❌ | Confirmed that it does not work; but probably can be made to work |

`xolotl` has been tested on `MATLAB 9.3.0.713579 (R2017b)`, but other versions should work too. 

### Installation 

Get this repo from within `MATLAB` using my package manager:

```
% copy and paste this code in your MATLAB prompt
urlwrite('http://srinivas.gs/install.m','install.m'); 
install sg-s/srinivas.gs_mtools % you'll need this
install sg-s/puppeteer % for manipulation
install sg-s/xolotl
```

or use git if you plan to develop this further: 

```
git clone https://github.com/sg-s/srinivas.gs_mtools
git clone https://github.com/sg-s/puppeteer
git clone https://github.com/sg-s/xolotl
```

Finally, make sure you [configure MATLAB so that it is set up to delete files permanently](https://www.mathworks.com/help/matlab/ref/delete.html). Otherwise you will end up with a very large number of temporary files in your trash!


## Benchmarks

These benchmarks were computed from within MATLAB, running the simulation at a time resolution of `50 us`. Speed is reported in multiples of real-time performance (so 10X = 10 seconds of simulation takes 1 s of real time).

| Model          | Paper reference | Hardware  | OS | Speed | 
| -------          | ------- | ----------- | ------ | -- |
| 7-conductance point neuron  | [Prinz 2003](http://jn.physiology.org/content/90/6/3998)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7    | macOS 10.12.6 | 83X | 
| 7-conductance point neuron  | [Prinz 2003](http://jn.physiology.org/content/90/6/3998)  |  Mac Pro (Late 2013) 3.5 GHz Xeon  | macOS 10.12.6   | 83X | 
| 3-neuron STG network | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7  | macOS 10.12.6   | 22X | 
| 3-neuron STG network  | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  Mac Pro (Late 2013) 3.5 GHz Xeon  | macOS 10.12.6   | 23X | 

Why are these metrics similar even on apparently different hardware? Maybe the Mac Pro just sucks. 

This code may be a lot faster if optimized correctly. 

## Developing 

### Tests

You are strongly encouraged to use [git hooks](https://git-scm.com/docs/githooks) to make sure that the code you write actually compiles. Look at this [example git hook](dev/pre-commit) which attempts to compile a bunch of C++ wrappers before permitting you to commit code. 



## License 

GPL v3

