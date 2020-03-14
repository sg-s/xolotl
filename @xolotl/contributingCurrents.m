
% 
% ### contributingCurrents
%
% **Syntax**
%
% ```matlab
% curr_index = xolotl.contributingCurrents(V, I)
% ```
%
% **Description**
%
% This static method calculates the contributions of each
% current at every point in a voltage race. This is used
% internally in `xolotl.plot` to color voltage traces.
%
% where V is a vector of voltages, I is the corresponding matrix of currents
%
% See Also: 
% xolotl.plot
% xolotl.manipulate



function curr_index = contributingCurrents(V, I)

corelib.assert(isvector(V),'V should be a vector ')

dV = [0; diff(V)];
Vsign = dV > 0;
curr_index = NaN * Vsign;
[~, curr_index(Vsign)] = min(I(Vsign,:)');
[~, curr_index(~Vsign)] = max(I(~Vsign,:)');
