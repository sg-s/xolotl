%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### unpack
%
%
% **Syntax**
%
% ```matlab
% p.unpack(all_data)
% ```
%
% **Description**
%
% Unpacks data from `all_data` and turns it into variables,
% as defined in `p.sim_func`.
%
%

function varargout = unpack(self, all_data)


corelib.assert(~isempty(self.sim_func),'No sim_func defined')

var_names = corelib.argOutNames(self.sim_func);

corelib.assert(length(all_data) == length(var_names),'Length of all_data does not match the number of outputs in the sim_func')

for i = 1:length(all_data)
	assignin('base',strtrim(var_names{i}),all_data{i})
end
