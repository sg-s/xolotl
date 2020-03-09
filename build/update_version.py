from datetime import date
import os
today = date.today()


# delete old version file
os.remove('+xolotl/version.m')

out_file = open('+xolotl/version.m','a+')

out_file.write('function version()')
out_file.write('\n')
temp = ("disp('v" + today.strftime('%y.%m.%d') + "');")
temp = temp.replace('.0','.')
out_file.write(temp)

out_file.close()