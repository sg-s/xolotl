%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: plots voltage traces
%
function plot(self)

comp_names = self.find('compartment');
N = length(comp_names);
c = lines(100);

if isempty(self.handles) || ~isfield(self.handles,'fig') || ~isvalid(self.handles.fig)
	if N == 1
		y = 500;
	else
		y = 900;
	end
	self.handles.fig = figure('outerposition',[0 0 1200 y],'PaperUnits','points','PaperSize',[1200 y]); hold on

	for i = 1:N
		self.handles.ax(i) = subplot(N,1,i); hold on
	end

	linkaxes(self.handles.ax,'x');

	% make all dummy plots
	
	for i = 1:N

		yyaxis(self.handles.ax(i),'left')
		cond_names = self.(comp_names{i}).find('conductance');
		for j = 1:length(cond_names)
			self.handles.plots(i).ph(j) = plot(self.handles.ax(i),NaN,NaN, 'Color', c(j,:),'LineWidth',3);
		end

		xlabel(self.handles.ax(i),'Time (s)')
		ylabel(self.handles.ax(i),['V_{ ' comp_names{i} '} (mV)'])

		% make calcium dummy plots
		yyaxis(self.handles.ax(i),'right')
		self.handles.Ca_trace(i) = plot(self.handles.ax(i),NaN,NaN,'Color','k');
		ylabel(self.handles.ax(i),['[Ca^2^+]_{' comp_names{i} '} (uM)'] )

		lh = legend([self.handles.plots(i).ph self.handles.Ca_trace(i)],[cond_names; '[Ca]']);
		lh.Location = 'eastoutside';

		for j = 1:length(self.handles.plots(i).ph)
			self.handles.plots(i).ph(j).Marker = 'none';
			self.handles.plots(i).ph(j).LineStyle = '-';
		end

		set(self.handles.ax(1),'XLim',[0 max(self.t_end*1e-3)]);

	end


end


[V, Ca, ~, currents] = self.integrate;
max_Ca = max(max(Ca(:,1:N)));

% process the voltage

time = 1e-3 * self.dt * (1:size(V,1));

a = 1;
for i = 1:N
	cond_names = self.(comp_names{i}).find('conductance');
	this_V = V(:,i);
	z = a + length(cond_names) - 1;
	this_I = currents(:,a:z);
	a = z + 1;

	dV = [0; diff(this_V)];
	Vsign = dV > 0;

	curr_index = NaN * Vsign;
	[~, curr_index(Vsign)] = min(this_I(Vsign,:)');
	[~, curr_index(~Vsign)] = max(this_I(~Vsign,:)');


	% show voltage
	for j = 1:size(this_I,2)
		Vplot = this_V;
		Vplot(curr_index ~= j) = NaN;
		self.handles.plots(i).ph(j).XData = time;
		self.handles.plots(i).ph(j).YData = Vplot;

	end


	% and now show calcium
	self.handles.Ca_trace(i).XData = time;
	self.handles.Ca_trace(i).YData = Ca(:,i);
	if isnan(max_Ca)
		set(self.handles.ax(i),'YLim',[0 1])
	else
		set(self.handles.ax(i),'YLim',[0 max_Ca])
	end


end

if strcmp(self.handles.ax(1).XLimMode,'auto')
	set(self.handles.ax(1),'XLim',[0 max(time)]);
	self.handles.ax(1).XLimMode = 'auto';
end

prettyFig('plw',1,'lw',1);