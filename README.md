# xolotl: a fast and flexible neuronal simulator

[![Documentation Status](https://readthedocs.org/projects/xolotl/badge/?version=master)](https://xolotl.readthedocs.io/en/master/?badge=master)
![](https://img.shields.io/github/last-commit/sg-s/xolotl.svg)


![](https://user-images.githubusercontent.com/6005346/57262966-9b751100-703c-11e9-890f-3808062e60d9.gif)




`xolotl` is a fast single-compartment and 
multi-compartment simulator  written in `C++` with 
a `MATLAB` interface that you'll actually enjoy using.

Why use xolotl? This is why:

-----------------

## xolotl is *FAST*

xolotl is written in C++, and it's **fast**. In our testing, it's 
more than **3 times** faster than NEURON for single
compartment neurons. 

![](https://user-images.githubusercontent.com/6005346/50499683-9c0bf400-0a19-11e9-9375-92a1fdefa2fc.png)

----------------
## xolotl is *easy to use*

Want to set up a Hodgkin-Huxley model, inject current, 
integrate it and plot the voltage trace? This is all you need:

```matlab
x = xolotl;
x.add('compartment', 'HH','A', 0.01);
x.HH.add('liu/NaV', 'gbar', 1000);
x.HH.add('liu/Kd', 'gbar', 300);
x.HH.add('Leak', 'gbar', 1);
x.I_ext = .2;
x.plot;
```


----------------

## xolotl has *documentation*

Unlike certain widely used NEURON simulators that 
shall remain nameless, xolotl has [documentation](https://xolotl.readthedocs.io) that actually...
exists. 

This is what it looks like:

![](https://user-images.githubusercontent.com/6005346/50499847-e3df4b00-0a1a-11e9-8aba-b3be57c3e784.png)


----------------
## xolotl is fully *programmable*

xolotl is designed to be used from **within MATLAB**. It 
gives you the best of both worlds: the high performance 
of C++ compiled code with the rich power of all the toolboxes
MATLAB has to offer. You can:

* write functions that pass models as arguments
* optimize parameters of neuron models using the [Global Optimization Toolbox](https://www.mathworks.com/products/global-optimization.html)
* run simulations in parallel across multiple computers
* have a single script to run the simulation and analyze results

--------------------

Hooked? [Get started here](https://xolotl.readthedocs.io/en/master/).

# Where do I get this?

Click [here](https://github.com/sg-s/xolotl/releases/download/latest/xolotl.mltbx) to download, and click on the downloaded file to install.


# How do I cite this?

We've published a technology report in [Frontiers in Neuroinformatics](https://doi.org/10.3389/fninf.2018.00087).

```
@ARTICLE{10.3389/fninf.2018.00087,
AUTHOR={Gorur-Shandilya, Srinivas and Hoyland, Alec and Marder, Eve},   
TITLE={Xolotl: An Intuitive and Approachable Neuron and Network Simulator for Research and Teaching},      
JOURNAL={Frontiers in Neuroinformatics},      
VOLUME={12},      
PAGES={87},     
YEAR={2018},      
URL={https://www.frontiersin.org/article/10.3389/fninf.2018.00087},       
DOI={10.3389/fninf.2018.00087},      
ISSN={1662-5196},   
}

```

![](https://user-images.githubusercontent.com/6005346/41205222-30b6f3d4-6cbd-11e8-983b-9125585d629a.png)


# Papers that use `xolotl`

1. [Regulation of Eag by Ca2+/calmodulin controls presynaptic excitability in Drosophila](https://www.physiology.org/doi/full/10.1152/jn.00820.2017)
2. [Homeostatic plasticity rules that compensate for cell size are susceptible to channel deletion](https://www.biorxiv.org/cgi/content/short/753608v1)