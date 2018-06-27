# get all files in methods directory 

 

import glob, os

out_file = open('auto_methods.rst','w')

header_str = '.. _method_name:'


for file in glob.glob("@xolotl/*.m"):

	filename = file.replace('.m','')
	filename = filename.strip()
	filename = filename.strip('@xolotl/')


	print(filename)

	if len(filename) == 0:
		continue

	lines = tuple(open(file, 'r'))

	a = -1
	z = -1


	for i in range(0,len(lines)):
		
		thisline = lines[i].strip('%')
		thisline = thisline.strip()

		if thisline == filename:
			a = i
			break

	for i in range(0,len(lines)):
		
		thisline = lines[i].strip('%')
		thisline = thisline.strip()
			
		if thisline.lower().find('function') != -1:
			z = i
			break


	if a < 0 or z < 0:
		continue

	

	this_header_str = header_str.replace('method_name',filename)
	out_file.write(this_header_str)
	out_file.write('\n\n')


	for i in range(a,z):
		thisline = lines[i]
		thisline = thisline.strip('%}')
		out_file.write(thisline)

	out_file.write('\n\nTest coverage\n')
	out_file.write('--------------\n\n')
	out_file.write('``' + filename + '`` is tested in: \n')

	# go over every file in /tests and check for this filename 
	for testfile in glob.glob("./tests/*.m"):
		if filename in open(testfile).read():

			test_filename = os.path.basename(testfile)

			out_file.write('\n`' + test_filename + ' <https://github.com/sg-s/xolotl/blob/master/%40xolotl/' + test_filename + '>`_ \n')
			


	out_file.write('\n\n\n')




out_file.close()


