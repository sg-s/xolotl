% 
% ### currentscape
%
% **Syntax**
%
% ```matlab
% x.currentscape_multi()
% [I_in, I_out, V] = x.currentscape_multi()
% ```
%
% **Description**
%
% This method creates a [currentscape](https://elifesciences.org/articles/42722), or a visualization of the fractional
% contribution of each current at each point in time 
% 
% When called with one output, no plot is created, but the raw normalized
% inward and outward currents are returned. 
%


function varargout = currentscape_multi(self,plot_here)

output_type=self.output_type;

self.output_type=1;
data = self.integrate;

for i = 1:numel(self.Children) % go over compartments
k=1;    
    Iion=self.(self.Children{i}).find('conductance');
    Is=self.(self.Children{i}).find('synapse');
    for j = 1:numel(Iion) % go over ionic currents
        I{i}(:,k)=data.(self.Children{i}).(Iion{j}).I; % ionic currents
        k=k+1;
    end
    for ii = 1:numel(Is) % go over synapses
        I{i}(:,k)=data.(self.Children{i}).(Is{ii}).state(:,1); % synaptic currents
        k=k+1;
    end
    V(i,:)=data.(self.Children{i}).V; % voltage
end

I_out = I; I_in = I;
for i = 1:numel(self.Children)
I_out{i}(I{i}<0) = 0; I_in{i}(I{i}>0) = 0;
I_out{i} = I_out{i}./sum(I_out{i},2); I_in{i} = I_in{i}./sum(I_in{i},2);
I_in{i} = -I_in{i}*100;
end

if nargout > 0
	varargout{1} = I_in;
    varargout{2} = I_out;
	varargout{3} = V;
	return
end

for i=1:numel(self.Children)
[~,idx{i}]=sort(sum(I_out{i}));
I_out{i} = I_out{i}(:,idx{i})*100;
size_I_out(i)=size(I_out{i},2);
end
%
C=colormaps.linspecer(max(size_I_out));
%C = colormaps.dcol(max(size_I_out));
%

make_axes = true;
% 
if nargin == 2
	if isa(plot_here,'matlab.graphics.axis.Axes') && isvalid(plot_here)
		ax.currentscape = plot_here;
		make_axes = false;

	end
end

if ~isfield(self.handles,'currentscape')
self.handles.currentscape=gobjects(1,numel(self.Children));
self.handles.currentscape_V=[]; 
self.handles.currentscape_in=[]; self.handles.currentscape_out=[]; 
end

for j=1:numel(self.Children) % go over compartments
    
    if isempty(self.handles) || ~isprop(self.handles.currentscape(j),'Name')
    
        if make_axes
            
            %self.handles.currentscape = figure('outerposition',[300 300 888 901],'PaperUnits','points','PaperSize',[888 901]); hold on
            self.handles.currentscape(j) = figure('outerposition',[300+j*100 100 888 750],'PaperUnits','points','PaperSize',[888 901]); hold on
            
            ax.voltage = subplot(4,1,1); hold on
            
            title(self.Children{j})
            
            ax.currentscape = subplot(4,1,2:4); hold on
            ylabel(ax.voltage,'V_m (mV)')
            
            ax.voltage.XColor = 'w';
           
            
            self.handles.currentscape_V(j) = plot(ax.voltage,self.time,data.(self.Children{j}).V,'k');
            
            ax.voltage.Position(3) = .6;
            
            drawnow
            
        end
        
        
        %ax.currentscape.YColor = 'w';
        ax.currentscape.YTick = [];
        
        a = area(ax.currentscape,self.time,squeeze(I_out{j}));
        for i = 1:length(a)
            a(i).EdgeColor = C(idx{j}(i),:);
            a(i).FaceColor = C(idx{j}(i),:);
        end
        
        self.handles.currentscape_in{j} = a;
        
        
        a = area(ax.currentscape,self.time,squeeze(I_in{j}));
        for i = 1:length(a)
            a(i).EdgeColor = C((i),:);
            a(i).FaceColor = C((i),:);
        end
        
        xlabel(ax.currentscape,'Time (s)')
        ylabel(ax.currentscape,'inward %          outward %')
        ylim([-100 100])
        
        hold on, line([0 self.t_end/10^3],[0 0],'color','k','Linewidth',1)
        
        if make_axes
            % fake plots for legend
            for i = 1:length(a)
                lh(i) = plot(NaN,NaN,'.','Color',C(i,:),'MarkerSize',33);
            end
            
            
            L = legend(lh,[self.(self.Children{j}).find('conductance');...
            self.(self.Children{j}).find('synapse')]);
            
            ax.currentscape.Position(3) = .6;
            
            drawnow
            
            L.Position(1) = .75;
            drawnow
            
            figlib.pretty('PlotLineWidth',1)
            
        end
        
        self.handles.currentscape_out{j}= a;
        
    else
        
            try
                self.handles.currentscape_V(j).YData = data.(self.Children{j}).V;
            catch
            end
            
            a = self.handles.currentscape_in{j};
            
            for i = 1:length(a)
                a(i).YData = I_in{j}(:,i);
                a(i).EdgeColor = C((i),:);
                a(i).FaceColor = C((i),:);
            end
            
            a = self.handles.currentscape_out{j};
            
            for i = 1:length(a)
                a(i).YData = I_out{j}(:,i);
                a(i).EdgeColor = C(idx{j}(i),:);
                a(i).FaceColor = C(idx{j}(i),:);
            end
        end
    end
    

self.output_type=output_type;