# this script converts all the markdown formatted
# documentation at the top of every method
# and links them together into a single 
# markdown documentation
 

import glob, os
from shutil import copyfile



copyfile('docs/reference/matlab/xolotl-head.md', 'docs/reference/matlab/xolotl.md')

out_file = open('docs/reference/matlab/xolotl.md','a+')


xolotl_method_root = '#';
cpplab_method_root = '../cpplab/#';

for file in sorted(glob.glob("@xolotl/*.m")):

	filename = file.replace('.m','')
	filename = filename.strip()
	filename = filename.replace('@xolotl/','')


	print(filename)

	if len(filename) == 0:
		continue

	lines = tuple(open(file, 'r'))

	a = -1
	z = -1


	for i in range(0,len(lines)):
		
		thisline = lines[i].strip('#')
		thisline = thisline.strip()

		if thisline == filename:
			a = i
			break

	for i in range(0,len(lines)):
		
		thisline = lines[i].strip('%')
		thisline = thisline.strip()
			
		if thisline.find('function') == 0:
			z = i
			break


	if a < 0 or z < 0:
		continue

	
	out_file.write('\n\n')
	out_file.write('-------\n')


	for i in range(a,z):
		thisline = lines[i]
		thisline = thisline.replace('%}','')

		# insert hyperlinks to other methods 
		if thisline.lower().find('->xolotl.') != -1:
			# ok, this is a xolotl method
			link_name = thisline.replace('->','')
			link_name = link_name.strip()
			method_name = thisline.replace('->xolotl.','')
			method_name = method_name.strip()
			method_name = method_name.lower()
			link_url = '[' + link_name + '](' + xolotl_method_root + method_name + ')'
			link_url = link_url.strip()
			link_url = '    * ' + link_url + '\n'
			out_file.write(link_url)
		elif thisline.lower().find('->cpplab.') != -1:
			# ok, this is a cpplab method
			link_name = thisline.replace('->','')
			link_name = link_name.strip()
			method_name = thisline.replace('->cpplab.','')
			method_name = method_name.strip()
			method_name = method_name.lower()
			link_url = '[' + link_name + '](' + cpplab_method_root + method_name + ')'
			link_url = link_url.strip()
			link_url = '    * ' + link_url + '\n'
			out_file.write(link_url)


		else:
			out_file.write(thisline)


	out_file.write('\n\n\n')

out_file.close()


# now the cpplab methods

copyfile('docs/reference/matlab/cpplab-head.md', 'docs/reference/matlab/cpplab.md')


f = open('../cpplab/docs/reference/cpplab-methods.md', "r")
cpplab_methods = f.read()
f.close()

fout = open('docs/reference/matlab/cpplab.md','a+')
fout.write(cpplab_methods)
fout.close()