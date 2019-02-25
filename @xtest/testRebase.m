function testRebase(self)

% check that every object has a valid C++ file 



x = self.single_comp;
x.rebase;

conds = x.find('conductance');

for i = 1:length(conds)
	this_cond = x.get(conds{i});

	self.assertTrue(strcmp(hashlib.md5hash(this_cond.cpp_class_path,'File'),this_cond.hash),'Hashes did not match; rebase failed')

end