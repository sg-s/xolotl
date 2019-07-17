%        __ _ _
% __  __/ _(_) |_
% \ \/ / |_| | __|
%  >  <|  _| | |_
% /_/\_\_| |_|\__|
%
%
% ### binCost
%
% **Syntax**
%
% ```matlab
% c = xfit.binCost(allowed_range, actual_value)
% ```
%
% **Description**
%
% A static method that computes a quadratic cost
% when `actual_value` is outside of the minimum and maximum
% set by the 2-vector `allowed_range`, and returns zero otherwise.
% 
% This method can be used as part of a simulation function
% when it is important for neurocomputational properties of interest to
% fit within a given range, rather than necessarily fit a value perfectly.

function c = binCost(allowed_range,actual_value)


if isnan(actual_value)
	c = 1;
	return
end

w = (allowed_range(2) - allowed_range(1))/2;
m = (allowed_range(2) + allowed_range(1))/2;

if actual_value < allowed_range(1)
	d = m - actual_value;
	c = (1- (w/d));
elseif actual_value > allowed_range(2)
	d = actual_value - m;
	c = (1- (w/d));
else
	% no cost
	c = 0;
end
