%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% contributingCurrents
% ^^^^^^^^^^^^^^^^^^^^
%
% calculates the contributions of each current at every point in a voltage trace. This is used internally in ``xolotl.plot`` to color voltage traces. Syntax ::
%
%   curr_index = contributingCurrents(V, I)
%
% where V is a vector of voltages, I is the corresponding matrix of currents %

function curr_index = contributingCurrents(V, I)

assert(isvector(V),'V should be a vector ')

dV = [0; diff(V)];
Vsign = dV > 0;
curr_index = NaN * Vsign;
[~, curr_index(Vsign)] = min(I(Vsign,:)');
[~, curr_index(~Vsign)] = max(I(~Vsign,:)');