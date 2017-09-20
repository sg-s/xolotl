# xolotl: The Standard Network Simulator (STG Network Simulator)

`xolotl` is a fast single-compartment simulator written in C++, with MATLAB wrappers. 

## Usage

Set up a simple single-compartment neuron:

```c++
// Set up some conductances 
NaV gna(1000,e_na, mNa, hNa);
Kd gkd(1000,e_k, mKd);
Leak gleak(0,e_leak);

// create a compartment 
compartment HH(-70, .01, Cm, A, f, Ca_out, Ca_in, tau_Ca);

// add the conductances to the compartment 
HH.addConductance(&gna);
HH.addConductance(&gcat);

```

Integrate this using a time step of `50 us`:

```c++
HH.integrate(50e-3);

```

`xolotl` also makes it easy to set up networks of neurons, or even multi-compartment neurons (the same code works for both). For an example of a three neuron network with two types of chemical synapses, see [integrateSTG.cpp](integrateSTG.cpp)

## Installation

Get this repo from within `MATLAB` using my package manager:

```
% copy and paste this code in your MATLAB prompt
urlwrite('http://srinivas.gs/install.m','install.m'); 
install sg-s/xolotl
```

or use git

```
git clone https://github.com/sg-s/xolotl

```

### Compiling 

Compile from within MATLAB using:

```matlab
mex integrateN7.cpp;
```

I've also written a small standalone wrapper that you can run on any computer, without MATLAB:

```bash
g++ integrateN7_standalone.cpp 
```


## Benchmarks

| Model          | Paper reference | Hardware  | Speed | 
| -------          | ------- | ----------- | ------ 
| 7-conductance point neuron  | [Prinz 2003](http://jn.physiology.org/content/90/6/3998)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7     | 83X | 
| 7-conductance point neuron  | [Prinz 2003](http://jn.physiology.org/content/90/6/3998)  |  Mac Pro (Late 2013) 3.5 GHz Xeon     | 83X | 
| 3-neuron STG network | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7     | 22X | 
| 3-neuron STG network  | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  Mac Pro (Late 2013) 3.5 GHz Xeon     | 23X | 

Why are these metrics similar even on apparently different hardware? Maybe the Mac Pro just sucks. 

This code could be a lot faster if optimized correctly. 

## Developing 

### Roadmap

- [ ] Support for custom parameters
- [ ] Support for custom initial conditions
- [ ] Compatibility with neuron, and control from MATLAB using [puppeteer](https://github.com/sg-s/puppeteer)
- [ ] Electrical Synapses 
- [x] Chemical Synapses
- [x] Support for integrating networks

### Tests

You are strongly encouraged to use [git hooks](https://git-scm.com/docs/githooks) to make sure that the code you write actually compiles. Look at this [example git hook](dev/pre-commit) which attempts to compile a bunch of C++ wrappers before permitting you to commit code. 



## License 

`xolotl` is loosely based on Tim O'Leary's C++ code, which you can see [here](https://github.com/marderlab/oleary_et_al_2014)

GPL v3

