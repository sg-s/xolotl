function manipulateEvaluateCond(self, thiscond, name, value)

thiscond.(name{1}) = value;

self.show(thiscond);

original_approx_state = self.approx_channels;

if self.pref.use_approx
	self.approx_channels = 1;
end

if isempty(self.manipulate_plot_func)
	self.plot();
else
	for i = length(self.manipulate_plot_func):-1:1
		self.manipulate_plot_func{i}(self);
	end
end

self.approx_channels = original_approx_state;
