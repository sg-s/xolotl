% this is an example of a custom plot function
% that can be used by manipulate
% this function generates a f-I curve from a xolotl
% object that is supposedly has one compartment 

function custom_fI(self)

all_I_ext = linspace(-.1,.3,20);

if isempty(self.handles) || ~isfield(self.handles,'fig') || ~isvalid(self.handles.fig)

	% no base figure, make it
	self.handles.fig = [];
	self.handles.fig = figure('outerposition',[100 100 600 600],'PaperUnits','points','PaperSize',[1200 600]); hold on

	self.handles.xh = plot(NaN,NaN,'ko');

	xlabel('I_{ext} (nA)')
	ylabel('Firing rate (Hz)')
	set(gca,'YLim',[-10 150],'XLim',[min(all_I_ext) max(all_I_ext)])

	prettyFig();


end


self.t_end = 1e3;
self.sim_dt = .1;
self.dt = .1;

all_f = NaN*all_I_ext;

for i = 1:length(all_I_ext)
	V = self.integrate(all_I_ext(i));
	all_f(i) = length(computeOnsOffs(V>0))/(self.t_end*1e-3);
	self.handles.xh.XData = all_I_ext;
	self.handles.xh.YData = all_f;
end

drawnow;