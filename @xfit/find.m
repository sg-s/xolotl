
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### find
%
% **Syntax**
%
% ```matlab
% xf.find()
% xf.find('MaxTime',1e6)
% xf.find('NModels',42)
% ```
%
% This method keeps finding solutions and saving them, and is designed to run indefinitely till you 
% manually stop it. 
%
%
%
% See Also: 
% xfit.fit
% xfit.save
% xfit.show



function find(self, varargin)

options.MaxTime = 1e6;
options.NModels = 1e3;

options = corelib.parseNameValueArguments(options,varargin{:});

goon = true;

tic
idx = 1;

while goon

	self.seed = [];

	if ~isempty(self.InitFcn)
		self.InitFcn(self.x,self.data);
	end

	self.fit()

	t = toc;

	idx = idx + 1;

	if t > options.MaxTime
		goon = false;
	end

	if idx > options.NModels
		goon = false;
	end

end