# xolotl: The Standard Network Simulator 

`xolotl` is a fast single-compartment simulator written in C++, with MATLAB wrappers. 

## Usage



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


## Benchmarks

5-second simulation of a 7-conductance neuron, as in Prinz et al. 2003:

~83X on a single thread on a Mac Pro (Late 2013)


## Roadmap

- [ ] Support for custom parameters
- [ ] Support for custom initial conditions
- [ ] Compataibility with neuron, and control from MATLAB using [puppeteer](https://github.com/sg-s/puppeteer)
- [ ] Electrical Synapses 
- [ ] Chemical Synapses
- [ ] Support for integrating networks


## License 

`xolotl` is loosely based on Tim O'Leary's C++ code, which you can see [here](https://github.com/marderlab/oleary_et_al_2014)

GPL v3

