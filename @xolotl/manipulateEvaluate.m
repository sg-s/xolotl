function manipulateEvaluate(self,parameters)

	self.updateLocalParameters(parameters);
	% evaluate 

	[V, Ca] = self.integrate;

	% update plots
	for i = 1:size(V,2)
		self.handles.V_trace(i).YData = V(:,i);
	end

end