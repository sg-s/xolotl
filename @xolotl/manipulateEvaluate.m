%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% callback for manipulate. used by puppeteer 
% when xolotl is being manipulated 

function manipulateEvaluate(self,names,values)

self.set(names,values)

self.manipulate_plot_func{1}(self);