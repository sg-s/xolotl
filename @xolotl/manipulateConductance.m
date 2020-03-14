
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### manipulateConductance
%
% This method is used to update the `xolotl` object
% every time a slider is moved in the manipulate window.
% This is used internally in `xolotl.manipulate`. You
% should not need to use this by itself. 
%
% See Also: 
% xolotl.manipulate
% xolotl.manipulateEvaluate





function manipulateConductance(self,thiscond)

rm_this = strcmp(thiscond.cpp_constructor_signature,'m') | strcmp(thiscond.cpp_constructor_signature,'h');
values = thiscond.get(thiscond.cpp_constructor_signature(~rm_this));
real_names = thiscond.cpp_constructor_signature(~rm_this);

% semi-intelligently make the upper and lower bounds
lb = values/3;
ub = values*3;
ub(values==0) = 1;


% make a snapshot of this now
self.snapshot('manipulate_zero')

self.show(thiscond);


% create a puppeteer instance and configure
warning('off','MATLAB:hg:uicontrol:ValueMustBeInRange')
warning('off','MATLAB:hg:uicontrol:MinMustBeLessThanMax')
p = puppeteer(real_names,values,lb,ub,[]);
self.handles.puppeteer_object = p;


self.reset('manipulate_zero')

if isempty(self.manipulate_plot_func)
	self.plot();
else
	for i = length(self.manipulate_plot_func):-1:1
		self.manipulate_plot_func{i}(self);
	end
end



warning('on','MATLAB:hg:uicontrol:MinMustBeLessThanMax')
warning('on','MATLAB:hg:uicontrol:ValueMustBeInRange')

p.continuous_callback_function = @(x,y) self.manipulateEvaluateCond(thiscond,x,y);
