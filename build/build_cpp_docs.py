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


import glob


xolotl_method_root = 'https://xolotl.readthedocs.io/en/master/reference/matlab/xolotl/#'
cpplab_method_root = 'https://xolotl.readthedocs.io/en/master/reference/matlab/cpplab/#'


# first, build headers for all class files
print("Generating headers...")


classnames = ["mechanism","conductance","network","compartment","synapse"];



for classname in classnames:

    outfile = open('docs/reference/c++/' + classname + '.md','w+')
    infile = 'c++/' + classname + '.hpp'

    lines = tuple(open(infile, 'r'))

    # show the initial comment, if any
    if lines[0].find('/*') >= 0:
        for i in range(1,len(lines)):
            thisline = lines[i]
            thisline = thisline.strip()
            if thisline.find('*/') >= 0:
                break
            outfile.write(thisline + '\n')

    outfile.write('\n## Methods\n\n')
    outfile.close()

# next, go over every .hpp file, and pull out all
# methods that may exist here. remember, a method in 
# a .hpp file may belong to any class, so we will have to figure 
# that out


print("Reading out methods of all classes...")


for file in sorted(glob.glob("c++/*.hpp")):


    print(file)
    lines = tuple(open(file, 'r'))

    for classname in classnames:

        outfile = open('docs/reference/c++/' + classname + '.md', 'a+')

        # find all methods defined here
        method_names = []
        method_line_def = []
        description_start = []
        comment_start = []
        for i in range(0, len(lines)):

            thisline = lines[i]
            thisline = thisline.strip()

            if thisline.find('/*') >= 0:
                comment_start = i

            if thisline.find(classname + "::") >= 0:
                a = thisline.find(classname + "::")
                z = thisline.find("(")
                method_names.append(thisline[a+2+len(classname):z])
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

                    if write_this.find('#') > 0:
                        a = write_this.find('#')
                        replace_me = write_this[a:].lower()
                        write_this = write_this[0:a] + replace_me

                    write_this = write_this.replace('/*','')
                    write_this = write_this.replace('*/','')
                    outfile.write(write_this)


            outfile.write('\n\n **Code**\n\n')
            outfile.write("[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/" + file + "#L" + str(line_no+1) + ")")
            outfile.write('\n\n-------\n\n')

        outfile.close()
