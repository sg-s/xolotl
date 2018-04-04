%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: integrates, and generates UI to manipulate parameters

function manipulate(self)


if ~isempty(self.linked_binary)
	self.skip_hash_check = true;
else
	self.transpile;
	self.compile;
	self.skip_hash_check = true;
end

% disable closed loop
self.closed_loop = false;

[V,Ca] = self.integrate;
time = (1:length(V))*self.dt;

t_end = self.t_end;

compartment_names = self.find('compartment');

% create a window to show all the traces
self.handles.fig = figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 500],'CloseRequestFcn',@self.deleteManipulateFig); hold on
n = length(compartment_names);
max_Ca = max(max(Ca(:,1:n)));
for i = 1:n
	self.handles.ax(i) = subplot(n,1,i);

	% show voltage
	yyaxis(self.handles.ax(i),'left')
	self.handles.V_trace(i) = plot(self.handles.ax(i),time,V(:,i),'k');
	ylabel(self.handles.ax(i),['V_{' compartment_names{i} '} (mV)'] )
	set(self.handles.ax(i),'YLim',[-80 80])

	% and now show calcium
	yyaxis(self.handles.ax(i),'right')
	c = lines(3);
	self.handles.Ca_trace(i) = plot(self.handles.ax(i),time,Ca(:,i),'Color',c(2,:));
	ylabel(self.handles.ax(i),['[Ca^2^+]_{' compartment_names{i} '} (uM)'] )
	set(self.handles.ax(i),'YLim',[0 max_Ca])

end
linkaxes(self.handles.ax,'x');
prettyFig('plw',1.5,'lw',1);



[values, ~, is_relational, real_names] = self.serialize;

% skip some dynamical values
rm_this = [lineFind(real_names,'*dt'); lineFind(real_names,'*.m'); lineFind(real_names,'*.h'); lineFind(real_names,'synapses*.s')];

% manually remove all the V, Ca for each neuron 
for i = 1:length(real_names)
	for j = 1:n
		if strcmp(real_names{i}, [compartment_names{j} '.Ca'])
			rm_this = [rm_this; i];
		end
		if strcmp(real_names{i}, [compartment_names{j} '.V'])
			rm_this = [rm_this; i];
		end
	end
end

values(rm_this) = [];
is_relational(rm_this) = [];
real_names(rm_this) = [];


% semi-intelligently make the upper and lower bounds
lb = values/3;
ub = values*3;


% create a puppeteer instance and configure
p = puppeteer(real_names,values,lb,ub,[],true);

% we're going to override pupeteer's callback functions for sliders that correspond to relational parameters, and disable those sliders
% for i = 1:length(is_relational)
% 	ir = is_relational{i};
% 	if length(p.handles) > 1
% 		h = p.handles(i);
% 	else
% 		h = p.handles;
% 	end
% 	for j = 1:length(h.sliders)
% 		if ir(j)
% 			h.sliders(j).Enable = 'off';
% 			z = strfind(h.controllabel(j).String,'=');
% 			h.controllabel(j).String = [h.controllabel(j).String(1:z-1) ' (relative)'] ;
% 			% remove the callback functions
% 			h.sliders(j).Callback = [];
% 		else

% 		end
% 	end
% end


p.attachFigure(self.handles.fig);
p.callback_function = @self.manipulateEvaluate;
self.handles.puppeteer_object = p;



