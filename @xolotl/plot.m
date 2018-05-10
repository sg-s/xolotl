%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: plots voltage traces
%
function handles = plot(self,varargin)

comp_names = self.find('compartment');
N = length(comp_names);

if length(varargin) == 0
	if N == 1
		y = 500;
	else
		y = 900;
	end
	handles.fig = figure('outerposition',[0 0 1200 y],'PaperUnits','points','PaperSize',[1200 y]); hold on

	for i = 1:N
		handles.ax(i) = subplot(N,1,i); hold on
	end

	linkaxes(handles.ax,'x');

end



[V, Ca, ~, currents] = self.integrate;

% process the voltage

time = 1e-3 * self.dt * (1:size(V,1));
c = lines(100);
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



	counter = 0;
	for j = 1:size(this_I,2)
		Vplot = this_V;
		Vplot(curr_index ~= j) = NaN;
		handles.plots(i).ph(j) = plot(handles.ax(i),time, Vplot, 'Color', c(j,:),'LineWidth',3);
	end

	legend(handles.plots(i).ph,cond_names)
	xlabel(handles.ax(i),'Time (s)')
	ylabel(handles.ax(i),['V_{ ' comp_names{i} '} (mV)'])

end

set(handles.ax(1),'XLim',[0 max(time)]);