

Simply use `save` and `load`, since `xolotl` objects are *bona fide* MATLAB objects. 
```matlab
% save a model as a .mat file
save('myModelFile.mat', 'x');
% load a xolotl object from a .mat file
load('myModelFile.mat');
```
