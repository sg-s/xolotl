

Simply use `save` and `load`, since `xolotl` objects are *bona fide* MATLAB objects. 

```matlab
% save a model as a .mat file
save('myModelFile.mat', 'x');
% load a xolotl object from a .mat file
load('myModelFile.mat');
```

!!! warning "Moving data across computers"
    When you save a xolotl object created in one computer, and load it in another computer, you will first have to run `x.rebase()` to link it to the C++ files it needs, since they are not saved together with the variable. If you get an error during `rebase`, that means you're missing C++ files, or `cpplab` can't find them. 



## See Also

* [cpplab.rebase](https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#rebase)
* [xolotl.rebase](https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#rebase)