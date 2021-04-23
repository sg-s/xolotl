This document details how to generate a database of parameters and metrics of some neuron or network model using the `xfind` class.


[Database searches](https://journals.physiology.org/doi/full/10.1152/jn.00641.2003) have been shown to be an effective way of exploring the rich and high-dimensional parameter space of neuron models. 


### Goal

Our goal in this exercise is to define some neuron model, 
and then randomly pick parameters and see if it generates
action potentials. The goal is to create a large library of parameters and firing rates so we can quickly generate
a long list of possible models that fire at some desired
firing rate by looking it up in the database. 


### Setting up the `xfind` object

First, we create a xolotl object with the model we want 
to explore. Here, we will use built-in functions to generate 
a eight-conductance neuron model. We will also create an xfind object and put the xolotl object inside it.

```matlab
p = xfind;
p.x = xolotl.examples.neurons.BurstingNeuron;
```

Now, let's define the parameters we want to explore
and specify the bounds of the hypercube in parameter space
we want to explore

```matlab
p.ParameterNames = p.x.find('*gbar');
p.Upper = [1e3 200 200 10 500 1e3 10 1e3];
p.Lower = zeros(8,1);
```

Now we need to tell `xfind` how to sample this space. 
You are free to write your own sampling function, but for now
we can use a built-in sampling function that uniformly
randomly samples points within these bounds.

Note that if you write your own sampling function, it should
accept an `xfind` object and should set the parameters of the 
xolotl object inside it. 

```matlab
p.SampleFcn = @p.uniformRandom;
```

OK, now we have a way of uniformly and randomly sampling our
parameter space. What we need next is some function to 
evaluate the model at sample points. Here, we are going to
use the built-in `measureFiringRate` function, but you are
free to write and use your own. If you do, make sure your
function accepts a xolotl object and returns a vector of doubles as its 
only output. 

```matlab
p.SimFcn = @xolotl.firingRate;
```

OK, we have now defined our `SampleFcn` and our `SimFcn`. There's one more piece: we don't really want to save all the 
data that we get from our random sampler. For example, we don't really want to clog up our database with neurons that 
never spike (you may want to, but for this example, we don't).

So we need to define a `DiscardFcn` that tells `xfind` when to save parameters and outputs from `SimFcn`, and when to
skip them.

```
% we are using an anonymous function for this simple example
p.DiscardFcn = @(data) data <= 0;
```

That's it! 

### Finding models

Now we are ready to find parameters. If you have the parallel 
computing toolbox installed, you can find models efficiently 
using all cores using

```matlab
p.parallelSearch;
Starting workers...
Starting worker #8
Starting worker #7
Warning: Objects of class 'parallel.FevalFuture' cannot be saved to MAT files. 
Starting worker #6
Starting worker #5
Starting worker #4
Starting worker #3
Starting worker #2
Starting worker #1
```

Notice that control is returned immediately to you. That's because the heavy computation occurs in the background. To stop it, run:

```matlab
cancel(p.workers)
```

### Looking at solutions

To look at the solutions that `xfind` found, use:

```matlab
[params,data]=p.gather;
```
`params` will be a matrix of your parameters (in the order you defined `ParameterNames` in) and `data` will be a matrix of the outputs of your `SimFcn`. In this case, there is only one output from `SimFcn`, so `data` is a vector. 


### Under the hood

`xfind` streams data to disk using binary files with extension `.xfind`. The name of the file is the hash of the `xolotl` model, so make sure you wipe these files (or move them) if you are changing your `SimFcn`
