[![GitHub last commit](https://img.shields.io/github/last-commit/sg-s/xolotl.svg)]()
![](https://img.shields.io/badge/macOS_10.12.6-7/7-brightgreen.svg) 
![](https://user-images.githubusercontent.com/6005346/41205222-30b6f3d4-6cbd-11e8-983b-9125585d629a.png)

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
x.add('AB','compartment','Cm',10,'A',0.0628);
x.AB.add('prinz-approx/NaV','gbar',1000,'E',50);
x.AB.add('prinz-approx/Kd','gbar',1000,'E',-80);
x.AB.add('Leak', 'gbar', 1, 'E', -50);
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
| macOS 10.12.6 |  | no known issues |
| Manjaro 17.1.10 |  |    you may need a specific compiler, see [this](http://xolotl.readthedocs.io/en/latest/compilers.html) for details |
| Ubuntu 16.04.3 LTS |  |   you may need a specific compiler, see [this](http://xolotl.readthedocs.io/en/latest/compilers.html) for details |
| Ubuntu 18.04 LTS |  |   you may need a specific compiler, see [this](http://xolotl.readthedocs.io/en/latest/compilers.html) for details |
| Windows 7/10 |  | You will need the [MinGW](https://www.mathworks.com/matlabcentral/fileexchange/52848-matlab-support-for-mingw-w64-c-c++-compiler) compiler. |

`xolotl` has been tested on `MATLAB 9.3.0.713579 (R2017b)` and `9.4.0.813654 (R2018a)`, but other versions should work too.

### Installation

[Click here](https://xolotl.readthedocs.io/en/latest/installing.html)


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

`xolotl` is currently in development, and improving every day. If you would like
to contribute, or have a question, correction, or error, please check with our [guide](http://xolotl.readthedocs.io/en/latest/contributing.html).

### Tests

You are strongly encouraged to use [git hooks](https://git-scm.com/docs/githooks) to make sure that the code you write actually compiles. Look at this [example git hook](dev/pre-commit) which attempts to compile a bunch of C++ wrappers before permitting you to commit code.



## License

GPL v3

