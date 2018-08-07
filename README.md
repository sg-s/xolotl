# xolotl: a fast and flexible neuronal simulator
![](https://img.shields.io/badge/macOS_10.12.6-9/9-brightgreen.svg) 
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

```

That's it. To integrate it and see the time evolution of the voltage

```matlab
  V = x.integrate;
```

![](https://user-images.githubusercontent.com/30243182/42067394-0801376e-7b14-11e8-8eb3-b20b64ffaf9f.png)

``xolotl`` handles all of the compilation/linking/etc. for you.

### It's fast


Because ``xolotl`` is written in ``C++``, it's quite fast. Here are some benchmarks for a single-compartment Hodgkin-Huxley model_ with sodium, potassium, and passive leak conductances.

```matlab
  x.benchmark;
```

![](https://user-images.githubusercontent.com/30243182/42105769-9c1c407a-7b9f-11e8-86fb-8fc56eac9ff6.png)

### Manipulate models in real time


Networks in ``xolotl`` can be manipulated in real-time using the graphical interface. Any parameter in ``xolotl`` can be accessed by a slider. Simulations are displayed by a chosen visualization function. Here, we manipulate the synaptic maximal conductances of a [three neuron model](https://www.ncbi.nlm.nih.gov/pubmed/15558066) and display using the ``xolotl`` ``plot`` function.


![](https://user-images.githubusercontent.com/6005346/30785272-aef9fb44-a132-11e7-84a6-25fd8e58470a.gif)


### It's free to use, free to copy and free to improve 

`xolotl` is released under a permissive GPL license. `xolotl` is meant to make working with neuron models easier, and will always be free to use. 


# Where do I get this?

[Read the docs](https://xolotl.readthedocs.io/en/latest/) to find out how to install it and how to use it. 














