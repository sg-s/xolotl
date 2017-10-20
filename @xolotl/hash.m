%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% hash function for xolotl. 
% uses openssl sha1 and writes to
% the file system temporarily
% to calcualte the hash

function h = hash(self)


[~,names] = self.serialize;
h = {};
for i = 1:length(names)
	this_names = names{i};
	if iscell(this_names)
		for j = 1:length(this_names)
			h{end+1} = this_names{j};
		end
	end
end
lineWrite('temp.xolotl',h)

[~,h] = system('openssl sha1 temp.xolotl');
h = strtrim(strrep(h,'SHA1(temp.xolotl)=',''));
delete('temp.xolotl');

