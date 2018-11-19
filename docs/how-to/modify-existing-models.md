In this document we will learn how to inspect the parameters of an existing model, and how we can change them. `xolotl` comes with powerful tools to change large numbers of parameters using logical operators and wildcards in one fell swoop. 

### Using the `find` and `get` methods

The `find` and `get` functions are robust searching tools for accessing parameters in `xolotl`. They are especially useful when you want to change many parameters at once.

Both functions accept cells of character vectors or character vectors as arguments. The `find` function returns a list of all fields of the `xolotl` structure as a cell of character vectors that fit that description. The `get` function works the same way, except that it returns the actual values of those fields. Under the hood, the `get` function is really just an extension of `find`.

It is important to note that both functions will find all fields that match any part of the search query. You can use asterisks `*` to "wildcard" that is, mean "anything here." This is useful especially between parts you want to specifically match for (such as `'AB*gbar'`).

These functions also work "down the tree," meaning that they are functions of each compartment, conductance, and so on, and search only within the part of the tree specified. For example, `x.AB.find('*')` will fetch every property of the `AB` compartment, but none in other compartments.

Get all conductances in all compartments.
```matlab
>> x.find('cond') % x.find('condu'), x.find('conduct'), ... x.find('conductance') also work too

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

Get the values of some arbitrary properties.

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

## Changing parameters

### Using `x.set()`
