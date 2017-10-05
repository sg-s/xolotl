%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% callback for manipulate. used by puppeteer 
% when xolotl is being manipulated 

function manipulateEvaluate(self,parameters)

	self.updateLocalParameters(parameters);
	% evaluate 

	[V, Ca] = self.integrate;

	% update plots
	for i = 1:size(V,2)
		self.handles.V_trace(i).YData = V(:,i);
	end

end