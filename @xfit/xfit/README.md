# xfit

Force-fit [Xolotl](https://github.com/sg-s/xolotl) objects so that they satisfy an arbitrary set of constraints.

![](./images/bed.png)


# What?

![](https://user-images.githubusercontent.com/6005346/37410120-37d45896-2776-11e8-95b8-77353996d2a5.png)

Let's say you start with the neuron on the left. You're unhappy with it, because:

1. Its voltage troughs don't go down to -70 mV
2. The slow wave goes past -40 mV
3. Its spikes go below the slow wave
4. It doesn't have a burst frequency of .5 Hz
5. It doesn't have a duty cycle of .3

What if you could make this neuron do what you want it to? `xfit` fiddles with parameters in the model until it does what it should (on the right).

Right now, you can use three algorithms:

1. `patternsearch` which is deterministic, and is a glorified form of gradient descent
2. `particleswarm` which is based on how flocks of birds fly and avoid predators. It's stochastic
3. `ga` which stands for genetic algorithm, a type of stochastic gradient descent.

The bottom plot shows how these algorithms perform. Using `particleswarm`, we can go from the initial neuron to the target neuron in around 5 minutes on a quad-core laptop. Note that the genetic algorithm performs quite poorly here.

This also means that you can very efficiently generate neurons from random initial conditions. The following six bursting neurons were found in **6 minutes** from random initial conditions.


![](https://user-images.githubusercontent.com/6005346/37423634-bf55520c-2794-11e8-87b6-3c466da8df19.png)




# License

`xfit` is free software. GPL v3.
