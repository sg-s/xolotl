[![GitHub last commit](https://img.shields.io/github/last-commit/sg-s/xolotl.svg)]()

![](https://user-images.githubusercontent.com/6005346/30719217-a1e8d54a-9ef0-11e7-8889-c284ac13163e.png)

# xolotl: The Standard Network Simulator 

`xolotl` is a fast single-compartment and multi-compartment simulator written in C++, with MATLAB wrappers. `xolotl` is written with a focus on flexibility and speed. It can simulate single-compartment conductance-based neuron models, networks of these, and detailed multi-compartment models. Because it's written in C++, it's really fast (see [benchmarks](#benchmarks)). 

## Why use `xolotl`

### 1. It's fast

Because `xolotl` is written in C++, it's quite fast. Here are some benchmarks for a single-compartment Hodgkin-Huxley model with 7 conductnaces. Note that you can get speeds up to 300X realtime! 

![](https://user-images.githubusercontent.com/6005346/38319588-223ab634-3800-11e8-954c-845c0692d4a0.png)

### 2. It's easy to use

You can set up complex models of neurons and networks very efficently, using an intuitive language that is tightly coupled to the object-based architecture of the underlying C++ code. For example, here, we set up a compartment with some channels in it:

```matlab
x = xolotl;
x.add('AB','compartment','Cm',10,'A',0.0628,'vol',1,'phi',19,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',200);
x.AB.add('prinz-approx/NaV','gbar',1000,'E',50);
x.AB.add('prinz-approx/Kd','gbar',1000,'E',-80);

```

That's it! To integrate it and see the time evolution of the voltage, it's as simple as:

```matlab
V = x.integrate;
```
![](https://user-images.githubusercontent.com/6005346/30713658-ff96faf4-9edd-11e7-9db1-a2ca4f2f0567.png)


`xolotl` handles all the compilation, linking, etc. for you. 

### 3. Manipulate neuron models in real time


![](https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif)

### 4. Optimize neuron models really quickly

`xolotl` comes with a powerful optimization toolbox called [procrustes](https://github.com/sg-s/procrustes). 


## Get this

### Requirements

| OS          | Support | Comments | 
| -------          | ------- | ----------- 
| macOS 10.12.6 | ✓ | no known issues |
| Ubuntu 16.04.3 LTS | ✓ |   no known issues | 
| Windows 7 | ❌ | Windows not supported currently. ~~You will have the [MinGW](https://www.mathworks.com/matlabcentral/fileexchange/52848-matlab-support-for-mingw-w64-c-c++-compiler) compiler. Hashing not supported; make sure you run `x.cleanup` before every `transpile`.~~ |

`xolotl` has been tested on `MATLAB 9.3.0.713579 (R2017b)`, but other versions should work too. 

### Installation 

[Click here](docs/INSTALLING.md)

[Make sure you have the right compilers](docs/compilers.md)


## Benchmarks

These benchmarks were computed from within MATLAB, running the simulation at a time resolution of `50 us`. Speed is reported in multiples of real-time performance (so 10X = 10 seconds of simulation takes 1 s of real time).

| Model          | Paper reference | Hardware  | OS | Speed | 
| -------          | ------- | ----------- | ------ | ---- |
| 7-conductance point neuron  | [Liu 1998](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7    | macOS 10.12.6 | 66X | 
| 7-conductance point neuron  | [Liu 1998](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)   |  Mac Pro (Late 2013) 3.5 GHz Xeon  | macOS 10.12.6   | 83X | 
| 7-conductance point neuron; approximate gating functions  | [Liu 1998](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)   |  Mac Pro (Late 2013) 3.5 GHz Xeon    | macOS 10.12.6 | 170X |  
| 3-neuron STG network | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  MacBook Pro (Late 2013) 2.3 GHz Core i7  | macOS 10.12.6   | 20X | 
| 3-neuron STG network  | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html)  |  Mac Pro (Late 2013) 3.5 GHz Xeon  | macOS 10.12.6   | 23X | 
| 3-neuron STG network; approximate gating functions | [Prinz 2004](https://www.nature.com/neuro/journal/v7/n12/full/nn1352.html) | MacBook Pro (Late 103) 2.3 GHz Core i7 | macOS 10.12.6   | 39X | 


## Developing 

### Tests

You are strongly encouraged to use [git hooks](https://git-scm.com/docs/githooks) to make sure that the code you write actually compiles. Look at this [example git hook](dev/pre-commit) which attempts to compile a bunch of C++ wrappers before permitting you to commit code. 



## License 

GPL v3

