# this script converts all the markdown formatted
# documentation at the top of every method
# and links them together into a single 
# markdown documentation
 

import glob, os
from shutil import copyfile

out_file = open('docs/reference/xolotl-methods.md','w')


xolotl_method_root = 'https://xolotl.readthedocs.io/en/master/reference/xolotl-methods/#';
cpplab_method_root = 'https://xolotl.readthedocs.io/en/master/reference/cpplab-methods/#';

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


# now copy over the docs from cpplab (assuming it exists)
copyfile('../cpplab/docs/reference/cpplab-methods.md','docs/reference/cpplab-methods.temp')

# replace all links to cpplab.readthedocs with xolotl.readthedocs
lines = tuple(open('docs/reference/cpplab-methods.temp', 'r'))

out_file = open('docs/reference/cpplab-methods.md','w')

for i in range(0,len(lines)):
	thisline = lines[i]

	thisline = thisline.replace('https://cpplab.readthedocs.io','https://xolotl.readthedocs.io')

	out_file.write(thisline)

out_file.close()

os.remove("docs/reference/cpplab-methods.temp")