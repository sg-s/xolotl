# this python scripts removes all
# ifndef import guards from C++ files 
# and replaces them with #pragma once

# find all C++ files
import os



for root, dirs, files in os.walk("../c++/mechanisms/"):
    for file in files:
        if file.endswith(".hpp"):
            thisfile = os.path.join(root, file)

            lines = tuple(open(thisfile, 'r'))

            outfile = open(thisfile + ".temp", 'w+')

            for line in lines:

                if line.find("#ifndef") >= 0:
                    outfile.write("#pragma once\n")
                elif  (line.find("#endif") >= 0) or (line.find("#define") >= 0):
                    print(line)
                else:
                    outfile.write(line)


            outfile.close()

            # delete the old file
            os.remove(thisfile)

            # rename the new file to the old file
            os.rename(thisfile + ".temp",thisfile)

