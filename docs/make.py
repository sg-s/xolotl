# this script converts all the markdown formatted
# documentation at the top of every method
# and links them together into a single
# markdown documentation


import glob, os
from shutil import copyfile





for file in sorted(glob.glob("./*/")):
	if file.find("@") < 0:
		continue

	classname =  file.replace('/','')
	classname =  classname.replace('@','')
	classname =  classname.replace('.','')
	print("--------------------------------------")
	print("Generating documentation for class: " + classname)


	# find the header file (which specified the props)
	header_file = './docs/reference/matlab/' + classname + "-head.md"
	doc_file = './docs/reference/matlab/' + classname + ".md"

	if not os.path.exists(header_file):
		print("[ABORT] Cant find header, skipping...")
		continue

	print('[OK] Header exists at: ' + header_file)

	# copy if over to a new file called classname.md
	copyfile(header_file, doc_file)

	# open the copy for writing
	out_file = open(doc_file,'a+')

	method_root = '../' + classname + '/#';


	# go over every method in this class
	for file in sorted(glob.glob("@" + classname + "/*.m")):


		filename = file.replace('.m','')
		filename = filename.strip()
		filename = filename.replace('@' + classname + '/','')


		if len(filename) == 0:
			continue

		print(filename)

		lines = tuple(open(file, 'r'))

		a = -1
		z = -1

		# read the file and figure out where the documentation
		# is, set a and z
		for i in range(0,len(lines)):
			thisline = lines[i].replace('#','')
			thisline = thisline.replace(' ','')
			thisline = thisline.replace('%','')
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


		print(a)
		print(z)
		out_file.write('\n\n')
		out_file.write('-------\n\n')

		format_link = False

		for i in range(a,z):
			thisline = lines[i]
			thisline = thisline.replace('%}','')
			thisline = thisline.strip('%')
			thisline = thisline.lstrip()
			thisline = thisline.replace('XXXX','    ')

			if not thisline:
				out_file.write('\n')
				continue

			# make the "see also" into a nice box
			if thisline.lower().find('see also') != -1:
				out_file.write('\n\n')
				thisline = '!!! info "See Also"\n'
				format_link = True
				out_file.write(thisline)
				continue

			# insert hyperlinks to other methods

			if thisline.find('* [') != -1 and format_link:
				# pre-formatted link, just write it
				out_file.write('    ' + thisline)

			elif thisline.lower().find(classname + '.') != -1 and format_link:
				# ok, this is a class method
				link_name = thisline
				link_name = link_name.strip()
				method_name = thisline.replace(classname +'.','')
				method_name = method_name.strip()
				method_name = method_name.lower()
				link_url = '[' + link_name + '](' + method_root + method_name + ')'
				link_url = link_url.strip()
				link_url = '    * ' + link_url + '\n'
				out_file.write(link_url)

			else:
				out_file.write(thisline)

out_file.write('\n\n\n')

out_file.close()







# now the cpplab methods. because cpplab exists in a differnet repo,
# we have to manually copy it from there

copyfile('docs/reference/matlab/cpplab-head.md', 'docs/reference/matlab/cpplab.md')


f = open('../cpplab/docs/reference/cpplab-methods.md', "r")
cpplab_methods = f.read()
f.close()

fout = open('docs/reference/matlab/cpplab.md','a+')
fout.write(cpplab_methods)
fout.close()
