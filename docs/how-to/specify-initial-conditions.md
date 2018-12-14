This document will describe how to specify initial conditions of a model. We will assume you already have a model set up. To quickly set up a model that we can work with, you can use:

```
xolotl.go_to_examples
demo_bursting_neuron;
```

# Initial conditions are specified by default

In general, components in xolotl are expected to specify their own default initial conditions. Thus, if you don't do anything, these default values are chosen. 

# Specify initial conditions by hand

You can set any property or initial condition of any component in your xolotl object manually. If you can see it, then you can modify it. For example, this is how you set the initial voltage your compartment:

```
x.CompName.V = -60; % mV
```

# Specify multiple initial conditions using the `set` method

In the model we created here, there are four channels that inactivate. We can set the inactivation variable in all four channels using:

```
x.set('AB.*h',0)
```

