
In this tutorial we will use the built-in demos in `xolotl` to see what
it can do, and to play with some models. We assume that you have installed
`xolotl` correctly, and have verified your installation. If you haven't
done that already, [click here and complete the steps on that page first](../tutorials/start-here.md).

### How to use the demos

First, let's go to the folder that contains the demos:

```matlab
xolotl.go_to_examples
```

Once there, you can run any of the scripts there to launch that demo. For example:

```matlab
demo_bursting_neuron
```

You should see this plot appear:

![](../images/bursting-neuron.png)

!!! info Something Went Wrong
    If this didn't work, see our [troubleshooting](../troubleshooting.md) page.
    You can also try running: `cpplab.rebuildCache` in MATLAB.

You can see what effect changing the maximal conductances has on the voltage dynamics by running:

```matlab
x.manipulate('*gbar')
```

This GUI will appear with sliders for maximal conductances of all the channels in the model. Try moving them around and seeing what happens with the voltage dynamics.

![](../images/manipulate.png)


### Available demos

Xolotl comes with several example scripts that illustrate various features of the simulator. They're all in a folder called "examples", and you can jump into this folder from anywhere using `xolotl.go_to_examples`. Once there, run any one of these scripts by typing its name.


| Demo name | What this demonstrates |
| --------- | ---------------------- |
| [demo_adaptive_sampling](https://github.com/sg-s/xolotl/blob/master/examples/demo_adaptive_sampling.m) | This example shows you how to use [`adaptive sampling`](https://github.com/sg-s/srinivas.gs_mtools/tree/master/%40adaptive) |
| [demo_approx](https://github.com/sg-s/xolotl/blob/master/examples/demo_approx.m) | This example shows you how to use the [`approx_channels`](https://xolotl.readthedocs.io/en/master/reference/xolotl/#approx_channels) property to speed up code execution |
| [demo_bursting_neuron](https://github.com/sg-s/xolotl/blob/master/examples/demo_bursting_neuron.m) | This example sets up a simple bursting neuron. It demonstrates how to use relational parameters to specify parameters that depend on other parameters in the model. |
| [demo_clamp](https://github.com/sg-s/xolotl/blob/master/examples/demo_clamp.m) | This example shows you how to voltage clamp a compartment, and simulate the process of estimating the I-V curve of a conductance |
| [demo_clamp_rk4](https://github.com/sg-s/xolotl/blob/master/examples/demo_clamp_rk4.m) | This demo is the same as `demo_clamp` except that it uses the [Runge-Kutta 4th order](../explanation/integration/#TheRunge-Kuttafourth-ordermethod) solver. |
| [demo_conductance](https://github.com/sg-s/xolotl/blob/master/examples/demo_conductance.m) | This script shows you how to use the conductance class to generate new C++ files for your own channels |
| [demo_controlling_integration](https://github.com/sg-s/xolotl/blob/master/examples/demo_controlling_integration.m) | This example shows you how to use the [`output_type`](https://xolotl.readthedocs.io/en/master/reference/xolotl/#output_type) property of xolotl to reduce memory use by finding spikes in the C++ code |
| [demo_integral_control](https://github.com/sg-s/xolotl/blob/master/examples/demo_integral_control.m) | This example shows you how to work with `mechanism` objects by adding homeostatic controllers to conductances and letting them tune the channel spectrum of a neuron |
| [demo_manipulate](https://github.com/sg-s/xolotl/blob/master/examples/demo_manipulate.m) | This demo shows you how to use the [manipulate](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#manipulate) method |
| [demo_multi_compartment](https://github.com/sg-s/xolotl/blob/master/examples/demo_multi_compartment.m) | This example sets up a multi-compartment model, where different compartments have different levels of channels |
| [demo_finite_size](https://github.com/sg-s/xolotl/blob/master/examples/demo_finite_size.m) | This example varies the size of a neuron, and shows that smaller neurons are more dominated by noise due to the finite number of ion channels |
| [demo_rk4](https://github.com/sg-s/xolotl/blob/master/examples/demo_rk4.m) | This demo shows you how to use the [`solver_order`](https://xolotl.readthedocs.io/en/master/reference/xolotl/#solver_order) property of xolotl to use a Runge-Kutta 4 ODE solver to simulate the model|
| [demo_pyloric_net](https://github.com/sg-s/xolotl/blob/master/examples/demo_pyloric_net.m) | In this example, we set up a small network of three neurons, demonstrating how synapses work in xolotl |
| [demo_stg_temperature](https://github.com/sg-s/xolotl/blob/master/examples/demo_stg_temperature.m)  | Here, we vary the temperature of the simulation, showing how we can make the model sensitive to temperature. |
