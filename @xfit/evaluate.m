% 
%        __ _ _
% __  __/ _(_) |_
% \ \/ / |_| | __|
%  >  <|  _| | |_
% /_/\_\_| |_|\__|
% 
% 
% ### evaluate
% 
% **Syntax**
% 
% ```matlab
% c = evaluate(self,params);
% ```
% 
% **Description**
% 
% Updates parameters in the xolotl object using
% `params` (a vector), evaluate the cost function,
% and return a cost (a double).
% 
% It is assumed that you have the following things
% configured in the xfit object:
% 
% - `x` (the xolotl object)
% - `SimFcn`
% - `parameter_names`
% 
% See Also:
% xfit.fit

function c = evaluate(self,params)
% update parameters in the xolotl object using x
for i = 1:length(self.parameter_names)
	self.x.set(self.parameter_names{i},params(i))
end
c = self.SimFcn(self.x, self.data);
