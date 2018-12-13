# this script converts all the markdown formatted
# documentation at the top of every method
# and links them together into a single 
# markdown documentation
 

import glob, os

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
			link_url = '[' + link_name + '](' + xolotl_method_root + method_name + ')'
			link_url = link_url.strip()
			link_url = '    ' + link_url + '\n'
			out_file.write(link_url)


		else:
			out_file.write(thisline)

	# out_file.write('\n\nTest coverage\n')
	# out_file.write('--------------\n\n')
	# out_file.write('``' + filename + '`` is tested in: \n\n')

	# # go over every file in /tests and check for this filename 
	# for testfile in glob.glob("./tests/*.m"):
	# 	if filename in open(testfile).read():

	# 		test_filename = os.path.basename(testfile)

	# 		out_file.write('- `' + test_filename + ' <https://github.com/sg-s/xolotl/blob/master/tests/' + test_filename + '>`_ \n')
			


	out_file.write('\n\n\n')




out_file.close()


