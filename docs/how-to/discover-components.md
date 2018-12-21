In this document we will learn how to discover existing components in `xolotl`.

### Hands-on exploration

Every component in `xolotl` exists a separate `C++` header file with a `*.hpp` extention. These files are organized in the `C++` folder in your `xolotl` folder. To know where this folder is, type this in your MATLAB terminal:

```matlab
[fileparts(fileparts(which('xolotl'))) filesep 'c++']
```

In this folder, you will find components [organized by type and then by first author](construct-models.md#whereshouldIputthem). For example, this is what the "conductances" folder looks like:

![](../images/folder-components.png)

Knowing what components are available is as simple as looking for yourself in this folder! `xolotl` was explicitly designed to make it easy to know *where* the C++ code your model was running was physically located on your computer, so that you can change it if you so choose.


For example, the delayed rectifier potassium conductance published in Liu *et al.* 1998 can be found in `../xolotl/c++/conductances/liu/Kd.hpp`.

### Using [`cpplab.search()`](https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#search)

The `cpplab` system that underlies `xolotl` can also search for all C++ header files using wildcards. For example, to find all components that are sourced from papers authored by Astrid Prinz, we can use:

```matlab
a = cpplab.search('prinz')

a =
  19×1 cell array
    {'conductances/prinz/ACurrent.hpp'             }
    {'conductances/prinz/CaS.hpp'                  }
    {'conductances/prinz/CaT.hpp'                  }
    {'conductances/prinz/HCurrent.hpp'             }
    {'conductances/prinz/KCa.hpp'                  }
    {'conductances/prinz/Kd.hpp'                   }
    {'conductances/prinz/NaV.hpp'                  }
    {'conductances/prinz-temperature/ACurrent.hpp' }
    {'conductances/prinz-temperature/CaS.hpp'      }
    {'conductances/prinz-temperature/CaT.hpp'      }
    {'conductances/prinz-temperature/HCurrent.hpp' }
    {'conductances/prinz-temperature/KCa.hpp'      }
    {'conductances/prinz-temperature/Kd.hpp'       }
    {'conductances/prinz-temperature/NaV.hpp'      }
    {'mechanisms/prinz/CalciumMech.hpp'            }
    {'synapses/prinz/Cholinergic.hpp'              }
    {'synapses/prinz/Glutamatergic.hpp'            }
    {'synapses/prinz-temperature/Cholinergic.hpp'  }
    {'synapses/prinz-temperature/Glutamatergic.hpp'}
```

Search for all fast, voltage-gated sodium channels.

```matlab
a = cpplab.search('NaV')

a =
  19×1 cell array
    {'custom/NaV.hpp'                 }
    {'dethier/NaV.hpp'                }
    {'kispersky/NaV.hpp'              }
    {'liu/NaV.hpp'                    }
    {'liu-euler/NaV.hpp'              }
    {'liu-temperature/NaV.hpp'        }
    {'nadim98/int1/NaV.hpp'           }
    {'nadim98/lg/NaV.hpp'             }
    {'nadim98/mcn1/NaV.hpp'           }
    {'odowd-aldrich/DmNaV.hpp'        }
    {'prinz/NaV.hpp'                  }
    {'prinz-temperature/NaV.hpp'      }
    {'rodriguez/Int1/NaV.hpp'         }
    {'soplata/reticular/NaV.hpp'      }
    {'soplata/thalamocortical/NaV.hpp'}
    {'soto-trevino05/NaV.hpp'         }
    {'traub/NaV.hpp'                  }
    {'turrigiano/NaV.hpp'             }
    {'wicher/DmNaV.hpp'               }
```


## See Also


* [cpplab.search](https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#search)

