% unpacks data from all_data 
% and makes variables 
% as defined in self.sim_func

function varargout = unpack(self, all_data)


assert(~isempty(self.sim_func),'No sim_func defined')

var_names = corelib.argOutNames(self.sim_func);

assert(length(all_data) == length(var_names),'Length of all_data does not match the number of outputs in the sim_func')

for i = 1:length(all_data)
	assignin('base',strtrim(var_names{i}),all_data{i})
end