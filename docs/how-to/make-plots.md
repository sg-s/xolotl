## Plot voltage by integrating the model

Since the outputs from `x.integrate` are matrices when `x.output_structure = 0`, you can plot them as you would any other matrix of vector in MATLAB. 

### Example

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


## Making plots using the [`plot`](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot) function
The [`plot`](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot) function will generate a figure, simulate the model, and plot the
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

### Example

```matlab
x.plot
```

makes this figure:

![](https://user-images.githubusercontent.com/6005346/50520224-1c1f7180-0a8d-11e9-9825-d42869338d33.png)

## Show activation curves of channels 

The activation curves and timescale dependence on any channel can be 
plotted using the [show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show) method. 

### Example

```matlab
xolotl.show('liu/NaV')
xolotl.show('prinz/NaV')
```


makes this figure:

![](https://user-images.githubusercontent.com/6005346/50520304-951ec900-0a8d-11e9-8ec1-88398062da2c.png)

## Make a stem plot of maximal conductances in a compartment

xolotl comes with a built in method to plot the maximal conductances
in a compartment. This can be useful for more complex visualizations
that you could make yourself.

### Example

```matlab
close all
x = xolotl.make_bursting_neuron;
x.plotgbars('AB')
```

makes a plot like this:

![](https://user-images.githubusercontent.com/6005346/50520444-6bb26d00-0a8e-11e9-82ba-128f08d28d30.png)

## See Also


* [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
* [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)
* [xolotl.plotgbars](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plotgbars)
* [xolotl.integrate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#integrate)
* [xolotl.pref](https://xolotl.readthedocs.io/en/master/reference/xolotl-properties/#pref)