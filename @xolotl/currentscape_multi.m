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

self.output_type=1;
data = self.integrate;

fn = fieldnames(data);

for i = 1:numel(fn) % go over the compartments
    fni = string(fn(i));
    field{i} = data.(fni);
    fn1 = fieldnames(field{i});
    
    k=1;
    for j=4:numel(fn1) % go over the currents
        fn1i = string(fn1(j));
        
        if isfield(field{i}.(fn1i),'I')==1
        I{i}(:,k) = field{i}.(fn1i).I; % ionic currents
        else
        I{i}(:,k) = field{i}.(fn1i).state(:,1); % synaptic current   
        end  
        k=k+1;
    end
end

for i=1:numel(I)
    
I_out{i} = I{i}; I_in{i} = I{i};
I_out{i}(I{i}<0) = 0; I_in{i}(I{i}>0) = 0;
I_out{i} = I_out{i}./sum(I_out{i},2); I_in{i} = I_in{i}./sum(I_in{i},2);
I_in{i}=-I_in{i};

end

time = (1:length(I{1}))*self.dt*1e-3;

if nargout > 0
	varargout{1} = I_in;
    varargout{2} = I_out;
	varargout{3} = V;
	return
end


for i=1:numel(I)
[~,idx{i}]=sort(sum(I_out{i}));
I_out{i} = I_out{i}(:,idx{i});
size_I_out(i)=size(I_out{i},2);
end

C=linspecer(max(size_I_out));
%C = colormaps.dcol(size(I_in{1},2));


make_axes = true;

if nargin == 2
	if isa(plot_here,'matlab.graphics.axis.Axes') && isvalid(plot_here)
		ax.currentscape = plot_here;
		make_axes = false;

	end
end


if isempty(self.handles) || ~isfield(self.handles,'currentscape') || ~isvalid(self.handles.currentscape)

    for j=1:numel(I)
        if make_axes
            
            %self.handles.currentscape = figure('outerposition',[300 300 888 901],'PaperUnits','points','PaperSize',[888 901]); hold on
            self.handles.currentscape = figure('outerposition',[300 300 888 550],'PaperUnits','points','PaperSize',[888 901]); hold on
            
            ax.voltage = subplot(4,1,1); hold on
            ax.currentscape = subplot(4,1,2:4); hold on
            ylabel(ax.voltage,'V_m (mV)')
            
            ax.voltage.XColor = 'w';
            
            self.handles.currentscape_V = plot(ax.voltage,time,field{j}.V,'k');
            
            ax.voltage.Position(3) = .6;
            
        end
        
        
        %ax.currentscape.YColor = 'w';
        
        a{j} = area(ax.currentscape,time,I_out{j});
        for i = 1:length(a{j})
            a{j}(i).EdgeColor = C(idx{j}(i),:);
            a{j}(i).FaceColor = C(idx{j}(i),:);
        end
        
        self.handles.currentscape_in = a{j};
        
        
        a{j} = area(ax.currentscape,time,I_in{j});
        for i = 1:length(a{j})
            a{j}(i).EdgeColor = C((i),:);
            a{j}(i).FaceColor = C((i),:);
        end
        
        xlabel(ax.currentscape,'Time (s)')
        ylabel(ax.currentscape,'inward %             outward %')
        ylim([-1 1])

    hold on, line([0 self.t_end/10^3],[0 0],'color','k','Linewidth',1)
    
	if make_axes
		% fake plots for legend
		for i = 1:length(a{j})
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

	self.handles.currentscape_out = a{j};


    end


else
    
    for j=1:numel(I)
        try
            self.handles.currentscape_V.YData = field{j}.V;
        catch
        end
        
        a{j} = self.handles.currentscape_in;
        
        for i = 1:length(a{j})
            a{j}(i).YData = I_in{j}(:,i);
            a{j}(i).EdgeColor = C((i),:);
            a{j}(i).FaceColor = C((i),:);
        end
        
        a{j} = self.handles.currentscape_out;
        
        for i = 1:length(a{j})
            a{j}(i).YData = I_out{j}(:,i);
            a{j}(i).EdgeColor = C(idx{j}(i),:);
            a{j}(i).FaceColor = C(idx{j}(i),:);
        end
    end
end