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
% help: use OpenSSL SHA1 to compute hash of xolotl configuration

function h = hash(self)

if self.skip_hash_check
	h = '0000000';
	return
end

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
z = strfind(h,'SHA1(temp.xolotl)=');
h = strtrim(h(z+19:end));
delete('temp.xolotl');

