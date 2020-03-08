## Plot voltage trace


### By directly integrating the model

Since the outputs from `x.integrate` are matrices when `x.output_structure = 0`, you can plot them as you would any other matrix of vector in MATLAB. 

For example,

```matlab
x = xolotl.make_bursting_neuron;
V = x.integrate;
time = x.dt:x.dt:x.t_end;
plot(time,V,'k')
xlabel('Time (ms)')
ylabel('V_m (mV)')
```
makes this figure:

![](https://user-images.githubusercontent.com/6005346/50520183-e11d3e00-0a8c-11e9-8f87-74eb6a98e672.png)


### Using the [`plot`](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#plot) function
The [`plot`](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#plot) function will generate a figure, simulate the model, and plot the
voltage traces. Here are a few important features to note:

* If a figure handle is set up in `x.handles.fig` it will use it. Otherwise, it will
generate the plot and put the figure handle there.
* Subplot handles are stores in `x.handles.ax`.
* The plots are colored by the largest contributing current. You can turn this
functionality off by setting `x.pref.plot_color = false`. To make this change
permanent for all xolotl objects, edit the `pref.m` file in the xolotl directory.
* The plots also show the calcium trace (if any). To turn this functionality off,
set `x.pref.show_Ca = false`. To make this change
permanent for all xolotl objects, edit the `pref.m` file in the xolotl directory.


!!! Note "What's a contributing current?"
    The voltage trace is colored by the dominant current at that time. When the voltage is increasing, the color corresponds to the largest positive (inward cation or outward anion) current. Inversely, when the voltage is decreasing, the color corresponds to the largest negative (outward cation or inward anion) current.

For example,

```matlab
x.plot
```

makes this figure:

![](https://user-images.githubusercontent.com/6005346/50520224-1c1f7180-0a8d-11e9-9825-d42869338d33.png)

## Show activation curves of channels 

The activation curves and timescale dependence on any channel can be 
plotted using the [show](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#show) method. 

For example,

```matlab
x = xolotl;
x.show('liu/NaV','Liu')
x.show('prinz/NaV','Prinz')
```


makes this figure:

![](https://user-images.githubusercontent.com/6005346/50520304-951ec900-0a8d-11e9-8ec1-88398062da2c.png)

`x.show` can also work with conductance objects directly. This is useful if you are working with a conductance object with custom runtime-defined parameters.


```matlab
x = xolotl.examples.BurstingNeuron;
x.show(x.AB.NaV)
```




## Make a stem plot of maximal conductances in a compartment

xolotl comes with a built in method to plot the maximal conductances
in a compartment. This can be useful for more complex visualizations
that you could make yourself.

For example,

```matlab
close all
x = xolotl.make_bursting_neuron;
x.plotgbars('AB')
```

makes a plot like this:

![](https://user-images.githubusercontent.com/6005346/50520444-6bb26d00-0a8e-11e9-82ba-128f08d28d30.png)

## See Also


* [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#plot)
* [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#show)
* [xolotl.plotgbars](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#plotgbars)
* [xolotl.integrate](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#integrate)
* [xolotl.pref](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#pref)