% this file contains preferences for xolotl
% do not edit the "defaults"  file
% if you want to specify your preferences,
% edit the pref.m file instead


%% ~~~~~~~~~~    plotting  ~~~~~~~~~~

% color voltage traces by the dominant 
% current? 
plot_color = false;

% show Calcium traces together
% with the voltage traces? 
show_Ca = true;


% should I plot gbars in a polar plot?
use_polar_gbar = true;


%% ~~~~~~~~~~  manipulate  ~~~~~~~~~~

% do you want to use approximate
% channels to speed up model interactions
% when using manipulate? 
% using approximations speeds up the integration
% and makes manipulate more responsive,
% but it can cause a seg fault
use_approx = true;


use_I_ext = true;


%% ~~~ caching ~~~~
% xolotl can cache outputs of integration to disk
% to speed up subsequent runs. This is useful in scripts that 
% make figures, where the same simulation is expected to 
% be run more than once
cache = false;



%% path to github CLI
gh_path = '/usr/local/bin/gh';
