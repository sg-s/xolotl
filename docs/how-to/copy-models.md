In this document we will learn how to create a copy of a xolotl model.

### Why you must use the `copy` method

Since `xolotl` objects derive from the [`handle` class of MATLAB](https://www.mathworks.com/help/matlab/matlab_oop/handle-objects.html), they do not behave like normal variables. Specifically, you cannot copy them by making assignations:

```matlab
% assuming x is a xolotl object
x2 = x; % x2 and x are actually the same thing
```

and changes in `x2` will automatically change `x` and vice versa.

Furthermore, xolotl will throw an error if you have created models with identical structure
but different compartment names.
If you need to instantiate two models simultaneously
that are identical in their component structure,
you should copy the model.

Therefore, if you want to make a true copy of a xolotl object, you must use its [`copy` method](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#copy):

```matlab
x2 = copy(x);
```

### See Also


* [xolotl.copy](https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#copy)
* [cpplab.copy](https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#copy)
