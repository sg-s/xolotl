% methods.m
% help: list all methods 
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% overloaded method called methods that shows the methods
% in xolotl 
function [] = methods(self)

m = builtin('methods',self);

fprintf(['Methods of xolotl:' '\n'])
fprintf(['================================================= \n'])

for i = 1:length(m)
	if exist([self.xolotl_folder,oss,'@xolotl',oss,m{i},'.m']) == 2
		% read the file and see if there is a help line 
		l = lineRead([self.xolotl_folder,oss,'@xolotl',oss,m{i},'.m']);
		if any(lineFind(l,'help:'))
			idx = (lineFind(l,'help:'));
			idx = idx(1);
			l = l{idx};
			l = strrep(l,'% help:','');
			S = [m{i} repmat(' ',1,25-length(m{i})) l];
			fprintf([S '\n'])
		end
	end
end