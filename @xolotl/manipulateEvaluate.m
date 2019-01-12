%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### manipulateEvaluate

This method is used to update the `xolotl` object 
every time a slider is moved in the manipulate window. 
This is used internally in `xolotl.manipulate`. You 
should not need to use this by itself. 

!!! info "See Also"
    ->xolotl.manipulate


%}


function manipulateEvaluate(self,names,values)

if any(strfind(names{1},'I_ext_'))
	comp_name = strrep(names{1},'I_ext_','');
	comp_idx = find(strcmp(self.Children,comp_name));

	self.I_ext(comp_idx) = values(1);

else
	self.set(names,values)
end


original_approx_state = self.approx_channels;

if self.pref.use_approx
	self.approx_channels = 1;
end

for i = 1:length(self.manipulate_plot_func)
	self.manipulate_plot_func{i}(self);
end

self.approx_channels = original_approx_state;