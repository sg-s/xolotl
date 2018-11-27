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

### Using `cpplab.search()`

The `cpplab` system that underlies `xolotl` can also search for all C++ header files using wildcards. For example, to find all components that are sourced from papers authored by Astrid Prinz, we can use:

```matlab
a = cpplab.search('prinz')

a =

  16×1 cell array

    {'/home/alec/code/xolotl/c++/conductances/prinz/ACurrent.hpp'            }
    {'/home/alec/code/xolotl/c++/conductances/prinz/CaS.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz/CaT.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz/HCurrent.hpp'            }
    {'/home/alec/code/xolotl/c++/conductances/prinz/KCa.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz/Kd.hpp'                  }
    {'/home/alec/code/xolotl/c++/conductances/prinz/NaV.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/ACurrent.hpp'}
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/CaS.hpp'     }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/CaT.hpp'     }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/HCurrent.hpp'}
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/KCa.hpp'     }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/Kd.hpp'      }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/NaV.hpp'     }
    {'/home/alec/code/xolotl/c++/synapses/prinz/Cholinergic.hpp'             }
    {'/home/alec/code/xolotl/c++/synapses/prinz/Glutamatergic.hpp'           }
```

Search for all fast, voltage-gated sodium channels.

```matlab
a = cpplab.search('NaV')

a =

  17×1 cell array

    {'custom/NaV.hpp'           }
    {'dethier/NaV.hpp'          }
    {'kispersky/NaV.hpp'        }
    {'liu/NaV.hpp'              }
    {'liu-euler/NaV.hpp'        }
    {'liu-temperature/NaV.hpp'  }
    {'nadim98/int1/NaV.hpp'     }
    {'nadim98/lg/NaV.hpp'       }
    {'nadim98/mcn1/NaV.hpp'     }
    {'odowd-aldrich/DmNaV.hpp'  }
    {'prinz/NaV.hpp'            }
    {'prinz-temperature/NaV.hpp'}
    {'rodriguez/Int1/NaV.hpp'   }
    {'soto-trevino05/NaV.hpp'   }
    {'traub/NaV.hpp'            }
    {'turrigiano/NaV.hpp'       }
    {'wicher/DmNaV.hpp'         }

```
