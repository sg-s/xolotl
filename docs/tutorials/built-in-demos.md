
In this tutorial we will use the built-in demos in `xolotl` to see what
it can do, and to play with some models. We assume that you have installed
`xolotl` correctly, and have verified your installation. If you haven't
done that already, [click here and complete the steps on that page first](../tutorials/start-here.md).

First, let's go to the folder that contains

```
xolotl.go_to_examples
demo_bursting_neuron
```


You should see this plot appear:

![](../images/bursting-neuron.png)

You can see what effect changing the maximal conductances has on the voltage dynamics by running:

```
x.manipulate('*gbar')
```

This GUI will appear with sliders for maximal conductances of all the channels in the model. Try moving them around and seeing what happens with the voltage dynamics.

![](../images/manipulate.png)

!!! note
    Xolotl comes with several example scripts that illustrate various features of the simulator. They're all in a folder called "examples", and you can jump into this folder from anywhere using `xolotl.go_to_examples`. `demo_stg` is a script that creates a three-cell network and integrates it. `demo_noise` introduces various types of noise into a bursting neuron and shows what the effect is. Run through all these examples to get a sense of what xolotl can do.
