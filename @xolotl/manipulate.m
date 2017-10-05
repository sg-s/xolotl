function manipulate(self)
	% create a window to show all the traces

	[V,Ca] = self.integrate;
	time = self.dt:self.dt:self.t_end;

	self.handles.fig = figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 500]); hold on
	n = length(self.compartment_names);
	for i = 1:n
		self.handles.ax(i) = subplot(n,1,i);
		self.handles.V_trace(i) = plot(self.handles.ax(i),time,V(:,i),'k');
		ylabel(self.handles.ax(i),['V_{' self.compartment_names{i} '} (mV)'] )
		set(self.handles.ax(i),'YLim',[-80 80])
	end
	linkaxes(self.handles.ax,'x');
	prettyFig('plw',1.5,'lw',1);
	
	% figure out the parameters -- one for each compartment 
	for i = 1:n
		[v,names] = struct2vec(self.(self.compartment_names{i}));
		% delete all E_, m_, and h_ parameters
		rm_this = false(length(v),1);
		for j = 1:length(v)
			if strcmp(names{j}(end-1:end),'_m') || strcmp(names{j}(end-1:end),'_h') || strcmp(names{j}(end-1:end),'_E') || strcmp(names{j}(end-1:end),'_V') 
				rm_this(j) = true;
			end
			if strcmp(names{j},'_Ca')
				rm_this(j) = true;
			end
		end
		v(rm_this) = []; names(rm_this) = [];

		% remove initial underscores from names
		names = cellfun(@(x) x(2:end),names,'UniformOutput',false);

		% reconstitute into a structure
		S = struct; U = struct; L = struct;
		for j = 1:length(v)
			S.(names{j}) = v(j);
			L.(names{j}) = v(j)/10;
			U.(names{j}) = v(j)*10;
		end 
		params{i} = S; lb{i} = L; ub{i} = U;
	end

	% and one more for the synapses 
	if length(self.synapses) > 0
		S = struct; U = struct; L = struct;
		for i = 1:length(self.synapses)
			this_name = [self.synapses(i).pre '_2_' self.synapses(i).post '_' self.synapses(i).type(1:4)];
			S.(this_name) = self.synapses(i).gbar;
			if self.synapses(i).gbar > 0
				U.(this_name) = self.synapses(i).gbar*5;
				L.(this_name) = 0;
			else
				L.(this_name) = -2*abs(self.synapses(i).gbar);
				U.(this_name) = 2*abs(self.synapses(i).gbar);
			end
		end

		params{end+1} = S;
		lb{end+1} = L;
		ub{end+1} = U;
	end

	if length(params) == 1
		params = params{1};
		lb = lb{1};
		ub = ub{1};
	end

	% create a puppeteer instance and configure
	p = puppeteer(params,lb,ub);
	p.attachFigure(self.handles.fig);

	p.callback_function = @self.manipulateEvaluate;
	if length(self.synapses) > 0
		p.group_names = [self.compartment_names; 'synapses'];
	else
		p.group_names = self.compartment_names;
	end


	self.handles.puppeteer_object = p;

	 
end % end manipulate 