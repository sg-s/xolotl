### Making plot using the outputs from `integrate`
Since the outputs from `x.integrate` are matrices, you can use all normal MATLAB
plotting functionality.

### Making plots using the `plot` function
The `x.plot` function will generate a figure, simulate the model, and plot the
voltage traces. Here are a few important features to note:

* If a figure handle is set up in `x.handles.fig` it will use it. Otherwise, it will
generate the plot and put the figure handle there.
* Subplot handles are stores in `x.handles.ax`.
* The plots are colored by the largest contributing current. You can turn this
functionality off by setting `setpref('xolotl', 'plot_color', false)`.
* The plots also show the calcium trace (if any). To turn this functionality off,
set `setpref('xolotl', 'show_Ca', false)`.

!!! Note "What's a contributing current?""
  The voltage trace is colored by the dominant current at that time. When the voltage
  is increasing, the color corresponds to the largest positive (inward cation or outward anion)
  current. Inversely, when the voltage is decreasing, the color corresponds to the largest
  negative (outward cation or inward anion) current.
