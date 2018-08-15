
disp('Attempting to download install script...')
try
	urlwrite('https://srinivas.gs/install.m','install.m');
catch
	error('Couldnt connect to the internet. If you''re seeing this message, the most likely explanation is that your MATLAB/OS is misconfigured and network requests are being blocked by a firewall. You will have to manually install xolotl.')
end

disp('Installing xolotl...')
install -f sg-s/xolotl

disp('Installing dependencies...')
install -f sg-s/srinivas.gs_mtools
install -f sg-s/cpplab
install -f sg-s/puppeteer 

disp('Compiling dependencies...')
GetMD5;