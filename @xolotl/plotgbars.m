
%               _       _   _
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### plotgbars
%
%
% **Syntax**
%
% ```matlab
% x.plotgbars('compartment_name');
% x.plotgbars(axes_handle,'compartment_name');
% axes_handle = x.plotgbars(axes_handle,'compartment_name');
% ```
%
% **Description**
%
% Makes a stem plot of conductance densities in a given compartment.
% If the first argument is a handle to a valid axis, plots will be made there.
% If no axis handle is given, it will use `gca` instead.
% This function can plot onto both Cartesian and polar axes.
%
%
% See Also:
% xolotl.plot
% xolotl.show
% xolotl.manipulate
%




function axes_handle = plotgbars(self,varargin)


if isempty(varargin)
	error('Not enough input arguments. You need to specify which compartment to plot maximal conductances for. ')
end


if length(varargin) == 2
	axes_handle = varargin{1};
	hold(axes_handle,'on')
	compartment_name = varargin{2};
elseif length(varargin) == 1
	axes_handle = gca; hold on
	compartment_name = varargin{1};
end



corelib.assert(isa(axes_handle,'matlab.graphics.axis.Axes') | isa(axes_handle,'matlab.graphics.axis.PolarAxes'),'Expected first argument to be a axes handle')
corelib.assert(any(strcmp(self.Children,compartment_name)),'compartment_name does not resolve to a known compartment')

channel_names = self.(compartment_name).find('conductance');

% clean up the channel_names a bit
for i = 1:length(channel_names)
	channel_names{i} = strrep(channel_names{i},'Current','');
end

g = self.(compartment_name).get('*gbar');

if ~isfield(self.handles,'gbar_plot') || ~isvalid(self.handles.gbar_plot(1))


	if self.pref.use_polar_gbar

		all_theta = 0:(2*pi/length(g)):2*pi;

		if isa(axes_handle,'matlab.graphics.axis.Axes')
			% delete and replace with a polaraxes
			p = axes_handle.Position;
			delete(axes_handle);
			axes_handle = polaraxes;
			axes_handle.Position = p;
			hold(axes_handle,'on')
		end

		m = min(log10(nonzeros(g/2)));
		M = max(log10(g*2));


		c = lines(length(g));

		for j = 1:length(g)
		    self.handles.gbar_stem(j) = polarplot(axes_handle,[all_theta(j) all_theta(j)],[m log10(g(j)) ],'Color',c(j,:),'LineWidth',2);
		    self.handles.gbar_cap(j) = polarscatter(axes_handle, all_theta(j),log10(g(j)));
		    self.handles.gbar_cap(j).MarkerFaceColor = c(j,:);
		    self.handles.gbar_cap(j).MarkerEdgeColor = c(j,:);
		    self.handles.gbar_cap(j).SizeData = 172;

		end

		axes_handle.RLim = [m M];

		axes_handle.ThetaTick = rad2deg(all_theta);
		axes_handle.ThetaTickLabels = self.(compartment_name).find('conductance');


	else

		for j = 1:length(g)
		    self.handles.gbar_plot(j) = stem(axes_handle,j,g(j));
		end


		set(axes_handle,'XTick',1:length(g),'XTickLabel',channel_names,'XTickLabelRotation',45,'YScale','log','XLim',[.5 length(g)+.5])
		ylabel(axes_handle,'g (uS/mm^2)')
	end


else
	% plot already exists, just update it
	for j = 1:length(g)
		self.handles.gbar_plot(j).YData = g(j);
	end
end
