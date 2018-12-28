function testGoToExamples(self)

original_dir = pwd;

xolotl.go_to_examples()
assert(any(strfind(pwd,'xolotl')) && any(strfind(pwd,'examples')),'Failed to go to example folder')

cd(original_dir)
