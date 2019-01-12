%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### plot

**Syntax**

```matlab
x.plot()
```

** Description**

`x.plot` makes a plot of voltage and calcium time series of all 
compartments. The default option is to color the voltage
traces by the dominant current at that point using  
`contributingCurrents` and to also show the Calcium 
concentration on the same plot. 


If you want to turn off the coloring, or to hide the 
Calcium concentration, change your preference using:


```matlab
x.pref.plot_color = false;
x.pref.show_Ca = false;
```

!!! info "See Also"
    ->xolotl.manipulate
    ->xolotl.contributingCurrents

%}


function plot(self, ~)

comp_names = self.find('compartment');
N = length(comp_names);
c = lines(100);

output_type = self.output_type;
self.output_type = 0;

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

	try
		linkaxes(self.handles.ax,'x');
	catch
	end

	% make all dummy plots
	
	for i = 1:N

		if self.pref.show_Ca
			yyaxis(self.handles.ax(i),'left')
		end
		
		cond_names = self.(comp_names{i}).find('conductance');

		if self.pref.plot_color

			for j = 1:length(cond_names)
				self.handles.plots(i).ph(j) = plot(self.handles.ax(i),NaN,NaN, 'Color', c(j,:),'LineWidth',2);
			end
		else
			self.handles.plots(i).ph = plot(self.handles.ax(i),NaN,NaN, 'Color', 'k','LineWidth',1.5);
		end

		xlabel(self.handles.ax(i),'Time (s)')
		if isnan(sum(self.V_clamp(:,i)))
			ylabel(self.handles.ax(i),['V_{ ' comp_names{i} '} (mV)'])
		else
			ylabel(self.handles.ax(i),['I_{clamp, ' comp_names{i} '} (nA)'])
		end

		% make calcium dummy plots
		
		if self.pref.show_Ca
			yyaxis(self.handles.ax(i),'right')
			self.handles.Ca_trace(i) = plot(self.handles.ax(i),NaN,NaN,'Color','k');
			ylabel(self.handles.ax(i),['[Ca^2^+]_{' comp_names{i} '} (uM)'] )
		end

		if self.pref.show_Ca & self.pref.plot_color
			lh = legend([self.handles.plots(i).ph self.handles.Ca_trace(i)],[cond_names; '[Ca]']);
			lh.Location = 'eastoutside';

			self.handles.lh = lh;
		end

		for j = 1:length(self.handles.plots(i).ph)
			self.handles.plots(i).ph(j).Marker = 'none';
			self.handles.plots(i).ph(j).LineStyle = '-';
		end

		set(self.handles.ax(1),'XLim',[0 max(self.t_end*1e-3)]);

		% attach this figure to the puppeteer instance 
		try
			self.handles.puppeteer_object.attachFigure(self.handles.fig);
		catch
		end

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

	curr_index = xolotl.contributingCurrents(this_V, this_I);

	% show voltage
	if self.pref.plot_color
		for j = 1:size(this_I,2)
			Vplot = this_V;
			Vplot(curr_index ~= j) = NaN;
			self.handles.plots(i).ph(j).XData = time;
			self.handles.plots(i).ph(j).YData = Vplot;

		end
	else

		self.handles.plots(i).ph(1).Color = 'k';
		self.handles.plots(i).ph(1).XData = time;
		self.handles.plots(i).ph(1).YData = this_V;
	end


	% and now show calcium
	if self.pref.show_Ca
		self.handles.Ca_trace(i).XData = time;
		self.handles.Ca_trace(i).YData = Ca(:,i);
		if isnan(max_Ca)
			set(self.handles.ax(i),'YLim',[0 1])
		else
			set(self.handles.ax(i),'YLim',[0 max_Ca])
		end
	end


end


if strcmp(self.handles.ax(1).XLimMode,'auto')
	set(self.handles.ax(1),'XLim',[0 max(time)]);
	self.handles.ax(1).XLimMode = 'auto';
end

try
	prettyFig('plw',1,'lw',1);
catch
end

self.output_type = output_type;