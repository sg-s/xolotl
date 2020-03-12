
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### manipulateEvaluate
%
% This method is used to update the `xolotl` object
% every time a slider is moved in the manipulate window.
% This is used internally in `xolotl.manipulate`. You
% should not need to use this by itself.
%
% See Also: 
% xolotl.manipulate
%




function manipulateEvaluate(self,names,values)

for i = 1:length(names)
	if any(strfind(names{i},'I_ext_'))
		comp_name = strrep(names{i},'I_ext_','');
		comp_idx = find(strcmp(self.Children,comp_name));

		if size(self.I_ext,1) > 1
			self.I_ext = zeros(1,length(self.Children));
		end
		self.I_ext(comp_idx) = values(i);

	else

		self.set(names{i},values(i))

		% do we have to mirror?
		if isfield(self.pref,'manipulated_params') && isfield(self.pref,'mirror_these')
			idx = find(strcmp(self.pref.manipulated_params,names{i}));
			self.set(self.pref.mirror_these{idx},values(i))
		end

	end
end




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
