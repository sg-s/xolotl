### Making plot using the outputs from `integrate`

Since the outputs from `x.integrate` are matrices when `x.output_structure = 0`, you can plot them as you would any other matrix of vector in MATLAB. 

### Making plots using the [`plot`](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot) function
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

!!! Note "What's a contributing current?
    The voltage trace is colored by the dominant current at that time. When the voltage
    is increasing, the color corresponds to the largest positive (inward cation or outward anion)
    current. Inversely, when the voltage is decreasing, the color corresponds to the largest
    negative (outward cation or inward anion) current.




## See Also


* [xolotl.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
* [xolotl.show](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#show)
* [xolotl.plotgbars](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plotgbars)
* [xolotl.integrate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#integrate)
* [xolotl.pref](https://xolotl.readthedocs.io/en/master/reference/xolotl-properties/#pref)