# xolotl: a fast and flexible neuronal simulator
<!--kapalin_test_result_start-->
![](https://img.shields.io/badge/macOS_10.13.6-9/9-brightgreen.svg) 
<!--kapalin_test_result_stop-->
![](https://readthedocs.org/projects/sphinx-rtd-theme/badge/?version=latest)
![](https://img.shields.io/github/last-commit/sg-s/xolotl.svg)

![](https://user-images.githubusercontent.com/6005346/41205222-30b6f3d4-6cbd-11e8-983b-9125585d629a.png)

`xolotl` is a fast single-compartment and multi-compartment simulator written in `C++` with a `MATLAB` interface. Designed with a focus on ease-of-use, flexibility and speed, `xolotl` simulates conductance-based neuron models and networks.

## Why Use `xolotl`?


### It's easy to use

You can set up complex models of neurons and networks very efficently, using an intuitive language that is tightly coupled to the object-based architecture of the underlying `C++` code.

For example, here, we set up a compartment with some channels in it: 

```matlab

  x = xolotl;
  x.add('compartment', 'HH', 'Cm', 10, 'A', 0.01);
  x.HH.add('liu/NaV', 'gbar', 1000);
  x.HH.add('liu/Kd', 'gbar', 300);
  x.HH.add('Leak', 'gbar', 0.1);
  x.I_ext = 0.1;
  x.t_end = 300; % ms

```

That's it. To integrate it and see the time evolution of the voltage, type:

```matlab
   x.plot
```

![](https://user-images.githubusercontent.com/6005346/44669694-24e53580-aa10-11e8-9bd0-5b0378d33342.png)

``xolotl`` handles all of the compilation/linking/etc. for you.

### It's fast


Because ``xolotl`` is written in ``C++``, it's quite fast. Here are some benchmarks for a single-compartment Hodgkin-Huxley model_ with sodium, potassium, and passive leak conductances. The built-in benchmarking tool can benchmark any model configured in it:

```matlab
   x.benchmark;
```


![](https://user-images.githubusercontent.com/6005346/44669774-6544b380-aa10-11e8-86e8-7bea1b2ad90b.png)

### Manipulate models in real time


Neurons and networks in ``xolotl`` can be manipulated in real-time using the graphical interface. Any parameter in ``xolotl`` can be accessed by a slider. Simulations are displayed by a chosen visualization function. Here, we manipulate the synaptic maximal conductances of a [three neuron model](https://www.ncbi.nlm.nih.gov/pubmed/15558066) and display using the ``xolotl`` ``plot`` function.


![](https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif)


### It's free to use, free to copy and free to improve 

`xolotl` is released under a permissive GPL license. `xolotl` is meant to make working with neuron models easier, and will always be free to use. 


# Where do I get this?

Click [here](https://drive.google.com/uc?id=15_Fy40Icnhbzfyz9eTqw-E0p5K9Lg_xu&export=download) to download, and click on the downloaded file to install. 
















