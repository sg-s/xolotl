This document describes how to manipulate a xolotl model. Here, we assume you have a simple (one or a few compartments) model whose parameters you want to explore. 

## Manipulating a model out of the box

All xolotl models can be manipulated out of the box. If you run


```matlab
x.manipulate
```

a window will pop up with sliders for every parameter, and 
[x.plot](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#plot)
is used to generate plots of voltage and calcium dynamics that will
auto-update as you move the sliders around.

To manipulate only a few parameters, you can call 
[manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate) with specific arguments:

```matlab
% only manipulate maximal conductances
x.manipulate('*gbar')

% only manipulate a specific conductance
x.manipulate('AB.NaV.E')

```

## Writing a custom plot function

You can also write your own functions that will make a plot that will update
as you vary the sliders created by x.manipulate. Your custom function should

* integrate the model and interpret outputs
* update or make plots as needed
* not change any parameters of the xolotl object 
* should not have any outputs
* should have only one input, a xolotl object

Let's assume your custom function is called `foo(x)`. 
To configure xolotl to use your custom function, use this syntax:


```matlab
x.manipulate_plot_func = {@foo};
```

Note that you can use this syntax to specify more than one plot function:

```matlab
x.manipulate_plot_func = {@foo, @bar};
```

Your custom function should look something like this:


```matlab
% note: no outputs, and only one input,
% which is a xolotl object
function foo(x)


% check if you need to make a figure. you can
% store handles to figures, and other data in
% x.handles
% this code snippet does the trick: 

if ~isfield(x.handles,'foo') ...
	|| ~isvalid(x.handles.foo.main_fig')

	% need to make the figure

	% insert code to make the figure here

	% save your figure handles in x.handles


end


% now you have to integrate the model
[V,Ca] = x.integrate;

% this code will be called every time the slider
% is moved. so make it snappy, and update plots
% efficiently

% the best way to do this is to update
% the XData and YData of your saved handles
% to plots


```

To see an example of this working, look at this 
[example file](https://github.com/sg-s/xolotl/blob/master/examples/demo_manipulate.m) that you can run using:


```matlab
xolotl.go_to_examples
demo_manipulate

```

!!! warning 
    The bigger your model is, the more sluggish manipulating it can appear. Try to use the simplest possible model to manipulate.


## See Also


* [xolotl.manipulate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulate)
* [xolotl.manipulateEvaluate](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#manipulateEvaluate)
* [puppeteer](https://github.com/sg-s/puppeteer)