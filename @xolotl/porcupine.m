%
% 				_       _   _
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### porcupine 
%
% **Syntax**
%
% ```matlab
% x.porcupine()
% data = x.porcupine
% ```
%
% ** Description**
%
% - **`x.porcupine`**  makes a plot of voltage and inward and outward currents
% in a polar plot, wrapped to the burst period of the neuron (assuming it bursts)
% - **`data = x.porcupine`**  does not make a plot, but returns a data structure indicating
% the direction and magnitude of the greatest contribution of each current
% 
% See Also: 
% xolotl.plot
% xolotl.currentscape
%


function varargout = porcupine(self)

corelib.assert(length(self.Children)==1,'porcupine only works with single-compartment models')


channels = self.(self.Children{1}).find('conductance');



self.reset;
self.t_end = 5e3;
self.integrate;
self.t_end = 10e3;
[V,~,~,I] = self.integrate;

time = (1:length(V))*self.dt*1e-3;

% preallocate outputs
t = NaN(length(channels),1);
r = t;

% find burst onsets 
spiketimes = self.dt*1e-3*veclib.nonnans(xtools.findNSpikeTimes(V,1e3));


if length(spiketimes) < 10
	disp('No spikes, aborting...')
	return
end

isis = diff(spiketimes);
burst_starts = spiketimes(find(isis>.3)+1);

% make a phase vector
phase = NaN*V;
for i = 1:length(burst_starts)-1
	a = round(burst_starts(i)/(self.dt*1e-3));
	z = floor(burst_starts(i+1)/(self.dt*1e-3));
	phase(a:z) = linspace(0,2*pi,z-a+1);
end
phase = phase(1:length(V));



c = lines;
c(8,:) = [1 0 0];
c(9,:) = [0 0 1];


if nargout == 0

	figure('outerposition',[30 30 850 1200],'PaperUnits','points','PaperSize',[850 1200]);
	subplot(3,3,1:3)
	plot(time,V,'k','LineWidth',1.5)
	xlabel('Time (s)')
	ylabel('V_m (mV)')
	set(gca,'YLim',[-90 60],'box','off','XLim',[0 2*mean(diff(burst_starts))])


	subplot(3,2,3);
	polarplot(phase,V,'k','LineWidth',2,'Color',[0 0 0])
	ax = gca;
	ax.ThetaDir = 'clockwise';
	ax.ThetaZeroLocation = 'top';
	ax.RLim = [-90  60];
	ax.RAxisLocation = 90;

	% now plot outward currents
	I_out = I;
	I_out(I_out<0) = NaN;
	subplot(3,2,4)
	polarplot(phase,log10(I_out(:,1)));
	hold on
	m = nanmin(log10(I_out(:))); 
	M = -nanmax(nanmin(log10(I_out(:))));
	for i = 1:length(channels)
		polarplot(phase,log10(I_out(:,i)),'LineWidth',2,'Color',c(i,:));

	end

	ax = gca;
	ax.ThetaDir = 'clockwise';
	ax.ThetaZeroLocation = 'top';
	ax.RAxisLocation = 90;
	ax.RLim = [m-(M-m)/10 M];
	title('log_{10}(Outward currents) (nA/mm^2)')

	% now plot in currents
	I_in = -I;
	I_in(I_in<0) = NaN;
	subplot(3,2,5)
	polarplot(phase,log10(I_in(:,1)));
	hold on
	clear l
	m = nanmin(log10(I_in(:))); 
	M = -nanmax(nanmin(log10(I_in(:))));
	for i = 1:length(channels)
		l(i) = polarplot(phase,log10(I_in(:,i)),'LineWidth',2,'Color',c(i,:));
	end

	ax = gca;
	ax.ThetaDir = 'clockwise';
	ax.ThetaZeroLocation = 'top';
	ax.RAxisLocation = 90;
	ax.RLim = [m-(M-m)/10 M];
	title('log_{10}(Inward currents) (nA/mm^2)')

	% finally we find all the average vectors and plot them
	subplot(3,2,6); 
	polarplot(NaN,NaN); hold on
	clear l
	for i = 1:length(channels)
		[xx,yy]=pol2cart(phase,abs(I(:,i)));
		xx = nanmean(xx); yy = nanmean(yy);
		[t(i),r(i)] = cart2pol(xx,yy);
		l(i) = polarplot([t(i) t(i)],[0 log10(r(i))],'LineWidth',4,'Color',c(i,:));
	end

	% show the ordering/legend
	[~,idx] = sort(t,1,'ascend');
	d = 0;
	for i = 1:length(channels)
		 th(i) = text(d,log10(2*max(r)),channels{idx(i)});
		 d = d + 2*pi/(length(channels));
		 th(i).Color = c(idx(i,:),:);
		 th(i).FontWeight = 'bold';
		 th(i).FontSize = 14;
	end


	ax = gca;
	ax.ThetaDir = 'clockwise';
	ax.ThetaZeroLocation = 'top';
	ax.RAxisLocation = 90;

	ax.ThetaTickLabel = {};

	figlib.pretty('PlotLineWidth',1.5);

else
	% no plot, just compute P
	clear l

	for i = 1:length(channels)
		[xx,yy]=pol2cart(phase,abs(I(:,i)));
		xx = nanmean(xx); yy = nanmean(yy);
		[t(i),r(i)] = cart2pol(xx,yy);
	end
end


data.theta = t(:);
data.radius = r(:);

if nargout == 1
	varargout{1} = data;
end