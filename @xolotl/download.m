
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### download
%
%
% **Syntax**
%
% ```matlab
% x.download;
% ```
%
% **Description**
%
% Downloads a pre-complied binary for
% this model, if available. 
%
%
%
%
%
% See Also: 
% xolotl.compile
% xolotl.viewCode
%

function download(self)

url = ['https://github.com/sg-s/xolotl/releases/download/bin/X_' self.hash '.' mexext];

savename = fullfile(filelib.cachePath('xolotl'),['X_' self.hash '.' mexext]);


try
	outfilename = websave(savename,url);
catch err

	corelib.assert(false,'Could not download a pre-complied binary for this model. You will have to manually compile yourself. You will need a configured C++ compiler for this.')

end

