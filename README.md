[![Build Status](https://travis-ci.org/sg-s/xolotl.svg?branch=master)](https://travis-ci.org/sg-s/xolotl)

![](https://user-images.githubusercontent.com/6005346/30719217-a1e8d54a-9ef0-11e7-8889-c284ac13163e.png)

# xolotl: The Standard Network Simulator 

`xolotl` is a fast single-compartment and multi-compartment simulator written in C++, with MATLAB wrappers. `xolotl` is written with a focus on flexibility and speed. It can simulate single-compartment conductance-based neuron models, networks of these, and detailed multi-compartment models. Because it's written in C++, it's really fast (see [benchmarks](#benchmarks)). 

## Usage (C++)

The simplest way to use `xolotl` is from C++. 

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

`xolotl` also makes it easy to set up networks of neurons, or even multi-compartment neurons (the same code works for both). For an example of a three neuron network with two types of chemical synapses, see [test_stg.cpp](tests/test_stg.cpp)

## Usage (MATLAB)

`xolotl.m` is a MATLAB class that generates and compiles C++ wrapper code on the fly, and uses that to run simulations, while presenting a pseudo-object oriented interface within MATLAB. 

Set up a `xolotl` instance:

```matlab
x = xolotl;
```

Set up a basic neuron and add some conductances:

```
x.addCompartment('AB',-70,0.05,10,0.0628,1.496,3000,0.05,200);
x.addConductance('AB','liu/NaV',1830,30);
x.addConductance('AB','liu/CaT',23,30);
x.addConductance('AB','liu/CaS',27,30);
x.addConductance('AB','liu/ACurrent',246,-80);
x.addConductance('AB','liu/KCa',980,-80);
x.addConductance('AB','liu/Kd',610,-80);
x.addConductance('AB','liu/HCurrent',10.1,-20);
x.addConductance('AB','Leak',.99,-50);
```

Inspect the neuron:

```
x.AB

ans = 

  struct with fields:

           V: -70
          Ca: 0.0500
          Cm: 10
           A: 0.0628
           f: 1.4960
      Ca_out: 3000
       Ca_in: 0.0500
      tau_Ca: 200
         NaV: [1×1 struct]
         CaT: [1×1 struct]
         CaS: [1×1 struct]
    ACurrent: [1×1 struct]
         KCa: [1×1 struct]
          Kd: [1×1 struct]
    HCurrent: [1×1 struct]
        Leak: [1×1 struct]

```

You can drill as deep as you want into the structure, and modify values if you want to. In this example, we're setting the reversal potential of the `NaV` channel on neuron `AB` to 50mV;

```
x.AB.NaV.E = 50; % mV
```


Integrate and plot the voltage:

```
x.dt = 50e-3;
x.t_end = 5000;
[V,Ca] = x.integrate;
figure, plot(V)
```

![](https://user-images.githubusercontent.com/6005346/30713658-ff96faf4-9edd-11e7-9db1-a2ca4f2f0567.png)

Behind the scene, MATLAB transpiles MATLAB code describing your network into C++, and compiles it using `mex` for your architecture. One of the advantages of the MATLAB interface is that you can manipulate all parameters of your neuron/network model, and see what the effect of that is live. This makes it much easier to get an intuition of how your model behaves. 

This is as simple as:

```matlab
x.manipulate;
```

![](https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif)

## Installation

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


## Benchmarks

These benchmarks were computed from within MATLAB, running the simulation at a time resolution of `50 us`. Speed is reported in multiples of real-time performance (so 10X = 10 seconds of simulation takes 1 s of real time).

| Model          | Paper reference | Hardware  | OS | Speed | 
| -------          | ------- | ----------- | ------ | -- |
| 7-conductance point neuron  | [Prinz 2003](http://jn.physiology.org/content/90/6/3998)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7    | macOS 10.12.6 | 83X | 
| 7-conductance point neuron  | [Prinz 2003](http://jn.physiology.org/content/90/6/3998)  |  Mac Pro (Late 2013) 3.5 GHz Xeon  | macOS 10.12.6   | 83X | 
| 3-neuron STG network | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7  | macOS 10.12.6   | 22X | 
| 3-neuron STG network  | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  Mac Pro (Late 2013) 3.5 GHz Xeon  | macOS 10.12.6   | 23X | 

Why are these metrics similar even on apparently different hardware? Maybe the Mac Pro just sucks. 

This code could be a lot faster if optimized correctly. 

## Developing 

### Tests

You are strongly encouraged to use [git hooks](https://git-scm.com/docs/githooks) to make sure that the code you write actually compiles. Look at this [example git hook](dev/pre-commit) which attempts to compile a bunch of C++ wrappers before permitting you to commit code. 



## License 

GPL v3

