In this document, we assume that we already
have a xolotl model set up. Here, we will learn how to

* modify parameters of a model one by one
* modify parameters of a model using pattern matching
* remove components from a model
* add new components to a model


## Modifying the parameters of an existing model

### Modify model parameter manually

Numerical parameters of xolotl objects, such as maximal
conductances can be changed by setting the value of
the parameter to the new parameter. For example, to
change the maximal conductance `gbar` of the `NaV`
conductance in the `AB` compartment of the `x` xolotl
object to $10~\mathrm{\mu S / mm^2}$, you can do:

```matlab
x.AB.NaV.gbar = 10;
```

### Modify model parameters using pattern matching


The `find` and `get` functions are robust searching tools for accessing parameters in `xolotl`. They are especially useful when you want to change many parameters at once.

Both functions accept cells of character vectors or character vectors as arguments. The `find` function returns a list of all fields of the `xolotl` structure as a cell of character vectors that fit that description. The `get` function works the same way, except that it returns the actual values of those fields. Under the hood, the `get` function is really just an extension of `find`.

It is important to note that both functions will find all fields that match any part of the search query. You can use asterisks `*` to "wildcard" that is, mean "anything here." This is useful especially between parts you want to specifically match for (such as `'AB*gbar'`).

These functions also work "down the tree," meaning that they are functions of each compartment, conductance, and so on, and search only within the part of the tree specified. For example, `x.AB.find('*')` will fetch every property of the `AB` compartment, but none in other compartments.

Get all conductances in all compartments.
```matlab
>> x.find('cond') 
% x.find('condu'), x.find('conduct'), 
% ... x.find('conductance') also work 

ans =

  8×1 cell array

    {'AB.ACurrent'}
    {'AB.CaS'     }
    {'AB.CaT'     }
    {'AB.HCurrent'}
    {'AB.KCa'     }
    {'AB.Kd'      }
    {'AB.Leak'    }
    {'AB.NaV'     }
```

Get the names of all conductances in the `AB` compartment.

```matlab
>> x.AB.find('conductance')

ans =

  8×1 cell array

    {'ACurrent'}
    {'CaS'     }
    {'CaT'     }
    {'HCurrent'}
    {'KCa'     }
    {'Kd'      }
    {'Leak'    }
    {'NaV'     }
```

Get the names of all maximal conductances in in `AB` compartment.

```matlab
>> x.find('AB*gbar')

ans =

  8×1 cell array

    {'AB.ACurrent.gbar'}
    {'AB.CaS.gbar'     }
    {'AB.CaT.gbar'     }
    {'AB.HCurrent.gbar'}
    {'AB.KCa.gbar'     }
    {'AB.Kd.gbar'      }
    {'AB.Leak.gbar'    }
    {'AB.NaV.gbar'     }
```

Get the names of all maximal conductances in the `AB` compartment from within the compartment's scope (note the importance of the wildcard `*`).

```matlab
>> x.AB.find('*gbar')

ans =

  8×1 cell array

    {'ACurrent.gbar'}
    {'CaS.gbar'     }
    {'CaT.gbar'     }
    {'HCurrent.gbar'}
    {'KCa.gbar'     }
    {'Kd.gbar'      }
    {'Leak.gbar'    }
    {'NaV.gbar'     }
```

Get the values of some arbitrary properties. In this case, we find the reversal
potentials of the `ACurrent` and the `HCurrent` conductances in the `AB` compartment.

```matlab
>> x.get('AB*Current.E')

ans =

   -80
   -20
```

And some more, using a cell of character vectors as the function argument.

```matlab
>> x.AB.NaV.get({'E', 'gbar'})

ans =

   30.0000
    0.1815
```

### Using `set` to change many parameters at once
Sometimes you want to set a bunch of parameters at once, such as all of the maximal
conductances, to values stored in a vector. You can do this line by line using
the "dot" notation as above, or use the `set` function to set all the parameters
at once.

The `set` function expects the same searching syntax as `find` and `get`, however
it also takes another argument which is the vector of values to assign to the
parameters.

If you wanted to set the maximal conductances of a single-compartment Hodgkin-Huxley
cell (`HH`) with fast sodium (`NaV`), delayed rectifier potassium (`Kd`), and `Leak`
conductances, you could set the parameters manually by

```matlab
x.HH.Kd.gbar = 300;
x.HH.Leak.gbar = 0.1;
x.HH.NaV.gbar = 1000;
```

or use the `set` function with the argument `'*gbar'`, meaning "all parameters of `x` ending
in `'gbar'`."

```matlab
x.set('*gbar', [300, 0.1, 1000]);
```


## Remove a component from a model

You can remove any component from the model by calling its `destroy` method. For example:

```matlab
x.CompName.CondName.destroy()
```


destroys an object called `CondName` from the compartment called `CompName`.

## Add a new component to an existing model

Xolotl objects are MATLAB objects. You can add new network components on-the-fly
by using the `add` function for compartments, conductances, and mechanisms, and
the `connect` function for synapses.



This value is now saved in the object structure. You can `save` the model, or
take a `snapshot` and this parameter's value will be maintained.

If you just wanted to see the parameter value (or store the value in another variable),
you can treat the expression `x.AB.NaV.gbar` just like any other variable.

```matlab
% print to the console
x.AB.NaV.gbar
```


## See Also

* [cpplab.set](https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#set)
* [cpplab.get](https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#get)
* [cpplab.find](https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#find)
* [cpplab.destroy](https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#destroy)
* [cpplab.add](https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#add)
* [xolotl.add](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#add)