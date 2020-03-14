
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### manipulateEvaluateCond
%
% This method is used to update the `xolotl` object
% every time a slider is moved in the manipulate window.
% This is used internally in `xolotl.manipulate`. You
% should not need to use this by itself. 
%
% See Also: 
% xolotl.manipulate
% xolotl.manipulateEvaluate

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
