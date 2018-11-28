# xolotl: a fast and flexible neuronal simulator

![](https://readthedocs.org/projects/sphinx-rtd-theme/badge/?version=latest)
![](https://img.shields.io/github/last-commit/sg-s/xolotl.svg)

![](https://user-images.githubusercontent.com/6005346/41205222-30b6f3d4-6cbd-11e8-983b-9125585d629a.png)

`xolotl` is a fast single-compartment and multi-compartment simulator written in `C++` with a `MATLAB` interface. Designed with a focus on ease-of-use, flexibility and speed, `xolotl` simulates conductance-based neuron models and networks.

## Why Use `xolotl`?


### It's easy to use

You can set up complex models of neurons and networks very efficiently, using an intuitive language that is tightly coupled to the object-based architecture of the underlying `C++` code.

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


Because ``xolotl`` is written in ``C++``, it's quite fast. Here are some benchmarks for a single-compartment Hodgkin-Huxley model with sodium, potassium, and passive leak conductances and another single-compartment model with [eight conductances](http://www.biology.emory.edu/research/Prinz/papers/article/5.pdf). The built-in benchmarking tool can benchmark any model configured in it:

```matlab
   x.benchmark;
```

![](https://camo.githubusercontent.com/c3275e7444b646e4a39cbc206f25da66efdae815/68747470733a2f2f7777772e66726f6e7469657273696e2e6f72672f66696c65732f41727469636c65732f3431393934312f666e696e662d31322d30303038372d48544d4c2f696d6167655f6d2f666e696e662d31322d30303038372d673030372e6a7067)

### Manipulate models in real time

Neurons and networks in ``xolotl`` can be manipulated in real-time using the graphical interface. Any parameter in ``xolotl`` can be accessed by a slider. Simulations are displayed by a chosen visualization function. Here, we manipulate the maximal conductances and reversal potentials of a Hodgkin-Huxley neuron model, simulate over a range of injected currents, and display using the ``xolotl`` ``plot`` function, and a firing rate over input (`fI`) function.


![](https://user-images.githubusercontent.com/30243182/49184986-b83f4700-f32e-11e8-9899-cd7e20e95fa9.gif)


### It's free to use, free to copy, and free to improve

`xolotl` is released under a permissive GPL license. `xolotl` is meant to make working with neuron models easier, and will always be free to use.

# Where do I get this?

Click [here](https://github.com/sg-s/xolotl/releases/download/latest/xolotl.mltbx) to download, and click on the downloaded file to install.

# How do I learn about this?

We use [Read the Docs](https://xolotl.readthedocs.io/en/master/) for our documentation.

# How do I cite this?

We've published a technology report in [Frontiers in Neuroinformatics](https://doi.org/10.3389/fninf.2018.00087).
