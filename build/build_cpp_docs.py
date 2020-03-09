# WHAT THIS IS
#
# This is a python script that generates documentation
# in markdown format from source C++ files that explains
# methods in C++ files in /xolotl/c++/
#
# HOW TO USE THIS
#
# In a terminal, go to the xolotl root folder and type:
# python make_cpp_docs.py
#
#
# WHO SHOULD USE THIS
#
# Use this only if you want to regenerate documentation
# from source C++ files. Only maintainers of xolotl
# should want to run this
#
# SEE ALSO
#
# build/build_docs.py


import glob, os
from shutil import copyfile


xolotl_method_root = 'https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#';
cpplab_method_root = 'https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#';

for file in sorted(glob.glob("c++/*.hpp")):


	classname = file.replace('.hpp','')
	classname = classname.strip()
	classname = classname.replace('c++/','')

	# does this classname end with a number? if so, skip
	if classname[-1].isdigit():
		continue

	print(classname)

	outfile = open('docs/reference/c++/' + classname + '.md','w+')


	if len(classname) == 0:
		continue

	# is there another file with the same name, but with
	# a number tacked on?
	ext_files = sorted(glob.glob("c++/" + classname + "*.hpp"))
	for ef in ext_files:
		efname = ef.replace('.hpp','')
		efname = efname.strip()
		efname = efname.replace('c++/','')
		if efname != classname:
			lines = tuple(open(file, 'r')) + tuple(open(ef,'r'))
		else:
			lines = tuple(open(file, 'r'))


	# show the initial comment, if any
	if lines[0].find('/*') >= 0:
		for i in range(1,len(lines)):
			thisline = lines[i]
			thisline = thisline.strip()
			if thisline.find('*/') >= 0:
				break
			outfile.write(thisline + '\n')

	outfile.write('\n## Methods\n\n')




	# find all methods defined here
	method_names = []
	method_line_def = []
	description_start = []
	comment_start = []
	for i in range(0,len(lines)):

		thisline = lines[i]
		thisline = thisline.strip()

		if thisline.find('/*') >= 0:
			comment_start = i

		if thisline.find(classname + "::") >= 0:
			a = thisline.find("::")
			z = thisline.find("(")
			method_names.append(thisline[a+2:z])
			method_line_def.append(i)
			description_start.append(comment_start)


	for method, line_no, desc in zip(method_names, method_line_def, description_start):

		outfile.write('\n\n')
		outfile.write('### '+ method + '\n\n')
		outfile.write('**Function Signature**\n\n')
		outfile.write('```C++\n')
		func_sig = lines[line_no]
		cutoff = func_sig.find('{')
		if cutoff > 0:
			func_sig = func_sig[0:cutoff]
		func_sig = func_sig.replace(classname+'::','')
		outfile.write(func_sig + '\n')
		outfile.write('```\n\n')
		outfile.write('**Description**\n\n')

		if desc:
			for i in range(desc, line_no):
				write_this = lines[i]
				write_this = write_this.replace('/*','')
				write_this = write_this.replace('*/','')
				outfile.write(write_this)


		outfile.write('\n\n **Code**\n\n')
		outfile.write("[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/" + classname + ".hpp#L" + str(line_no+1) + ")")
		outfile.write('\n\n-------\n\n')

	outfile.close()
