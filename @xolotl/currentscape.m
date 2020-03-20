% 
% ### currentscape
%
% **Syntax**
%
% ```matlab
% x.currentscape()
% norm_currents = x.currentscape()
% ```
%
% **Description**
%
% This method creates a "currentscape", or a visualization of the fractional
% contribution of each current at each point in time 
% 
% When called with one output, no plot is created, but the raw normalized
% inward and outward currents are returned. 
%
% This method only works with single-compartment models. 
%
% You can use manipulate to vary parameeters and see this update in real time
% 
% ```matlab
% x.manipulate_plot_func = {@x.currentscape};
% x.manipulate('*gbar');
% ```
%
%
% See Also: 
% xolotl.plot
% xolotl.manipulate
% [Visualization of currents in neural models with similar behavior and different conductance densities](https://elifesciences.org/articles/42722)

function varargout = currentscape(self,plot_here)


corelib.assert(length(self.Children)==1,'The currentscapes method only works for single-compartment models')


[V,~,~,I] = self.integrate;


time = (1:length(V))*self.dt*1e-3;

I_out = I;
I_in = I;

I_out(I<0) = 0;
I_in(I>0) = 0;


I_out = I_out./sum(I_out,2);
I_in = I_in./sum(I_in,2);

norm_currents = [I_in, I_out];

if nargout > 0
	varargout{1} = norm_currents;
	varargout{2} = V;
	return
end

I_in(:,1) = I_in(:,1) - 1;


[~,idx]=sort(sum(I_out));
I_out = I_out(:,idx);

C = colormaps.dcol(size(I_in,2));

make_axes = true;

if nargin == 2
	if isa(plot_here,'matlab.graphics.axis.Axes') && isvalid(plot_here)
		ax.currentscape = plot_here;
		make_axes = false;

	end
end


if isempty(self.handles) || ~isfield(self.handles,'currentscape') || ~isvalid(self.handles.currentscape)

	if make_axes

		self.handles.currentscape = figure('outerposition',[300 300 888 901],'PaperUnits','points','PaperSize',[888 901]); hold on

		ax.voltage = subplot(4,1,1); hold on
		ax.currentscape = subplot(4,1,2:4); hold on
		ylabel(ax.voltage,'V_m (mV)')

		ax.voltage.XColor = 'w';

		self.handles.currentscape_V = plot(ax.voltage,time,V,'k');

		ax.voltage.Position(3) = .6;

	end

	
	ax.currentscape.YColor = 'w';

	


	
	a = area(ax.currentscape,time,I_out);


	for i = 1:length(a)
		a(i).EdgeColor = C(idx(i),:);
		a(i).FaceColor = C(idx(i),:);
	end

	self.handles.currentscape_in = a;


	
	a = area(ax.currentscape,time,I_in);

	for i = 1:length(a)
		a(i).EdgeColor = C((i),:);
		a(i).FaceColor = C((i),:);
	end



	


	xlabel(ax.currentscape,'Time (s)')
	

	if make_axes
		% fake plots for legend
		for i = 1:length(a)
			lh(i) = plot(NaN,NaN,'.','Color',C(i,:),'MarkerSize',33);
		end


		L = legend(lh,self.(self.Children{1}).find('conductance'));

		ax.currentscape.Position(3) = .6;

		drawnow

		L.Position(1) = .75;
		drawnow

		figlib.pretty('PlotLineWidth',1)

	end

	self.handles.currentscape_out = a;







else

	try
		self.handles.currentscape_V.YData = V;
	catch
	end
	
	a = self.handles.currentscape_in;

	for i = 1:length(a)
		a(i).YData = I_in(:,i);
		a(i).EdgeColor = C((i),:);
		a(i).FaceColor = C((i),:);
	end

	a = self.handles.currentscape_out;

	for i = 1:length(a)
		a(i).YData = I_out(:,i);
		a(i).EdgeColor = C(idx(i),:);
		a(i).FaceColor = C(idx(i),:);
	end

end