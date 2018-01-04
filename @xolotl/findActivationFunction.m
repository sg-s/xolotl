%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: estimates activation function given Voltage, Current or conductances
%

function [] = findActivationFunction(varargin)

% options and defaults
options.V = [];
options.g = [];
options.I = [];
options.p = 1:5;
options.E_rev = NaN;


if nargout && ~nargin
	varargout{1} = options;
	return
end

% validate and accept options
if iseven(length(varargin))
	for ii = 1:2:length(varargin)-1
	temp = varargin{ii};
    if ischar(temp)
    	if ~any(find(strcmp(temp,fieldnames(options))))
    		disp(['Unknown option: ' temp])
    		disp('The allowed options are:')
    		disp(fieldnames(options))
    		error('UNKNOWN OPTION')
    	else
    		options = setfield(options,temp,varargin{ii+1});
    	end
    end
end
elseif isstruct(varargin{1})
	% should be OK...
	options = varargin{1};
else
	keyboard
	error('Inputs need to be name value pairs')
end


% check if conductances exist
if isempty(options.g)
	disp('need to convert currents to conductances')
	keyboard
end


boltz = @(A,B,x) (1./(1 + exp((x+A)./B)));

figure('outerposition',[30 30 1200 900],'PaperUnits','points','PaperSize',[1200 900]); hold on
for i = 1:length(options.p)
	autoPlot(length(options.p),i); hold on
	x = options.V;
	y = options.g; y = y/y(end);
	y = (y).^(1/options.p(i));
	plot(x,y,'k+')
	xlabel('V (mV)')
	ylabel(['g^{' oval(1/options.p(i),'frac') '}'])
	ff = fit(vectorise(x),vectorise(y),boltz,'StartPoint',[12.3, -11.8]);
	xx = linspace(min(x),max(x),1e3);
	plot(xx,ff(xx),'r')
	r2 = rsquare(ff(x),y);
	title(['1 - r^2 = ' oval(1-r2)])

	v = coeffvalues(ff);
	fprintf(['p = ' oval(i) ', A = ' oval(v(1),3) ', B = ' oval(v(2),3) ' \n'])
end

prettyFig();
