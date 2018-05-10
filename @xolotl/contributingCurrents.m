%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: computes most contributing currents for voltage trace
%
function curr_index = contributingCurrents(V, I)

dV = [0; diff(V)];
Vsign = dV > 0;
curr_index = NaN * Vsign;
[~, curr_index(Vsign)] = min(I(Vsign,:)');
[~, curr_index(~Vsign)] = max(I(~Vsign,:)');