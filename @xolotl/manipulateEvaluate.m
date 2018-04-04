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
	
	[V, Ca] = self.integrate;
	time = self.dt*(1:length(V));

	max_Ca = max(max(Ca(:,1:size(V,2))));

	% update plots
	for i = 1:size(V,2)
		yyaxis(self.handles.ax(i),'left')
		self.handles.V_trace(i).YData = V(:,i);
		self.handles.V_trace(i).XData = time;

		yyaxis(self.handles.ax(i),'right')
		self.handles.Ca_trace(i).YData = Ca(:,i);
		self.handles.Ca_trace(i).XData = time;
		self.handles.ax(i).YLim(2) = max_Ca;
	end


end