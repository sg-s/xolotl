%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: integrates, and generates UI to manipulate parameters

function manipulate(self)
	% create a window to show all the traces

	self.skip_hash_check = true;

	[V,Ca] = self.integrate;
	time = self.dt:self.dt:self.t_end;

	% if it's in closed loop, update it continuously
	t_end = self.t_end;
	buffer_size = 50; % ms
	buffer_chunk_size = floor(buffer_size/self.dt);
	if self.closed_loop
		self.t_end = buffer_size;
	end

	self.handles.fig = figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 500],'CloseRequestFcn',@self.deleteManipulateFig); hold on
	n = length(self.compartment_names);
	for i = 1:n
		self.handles.ax(i) = subplot(n,1,i);
		self.handles.V_trace(i) = plot(self.handles.ax(i),time,V(:,i),'k');
		ylabel(self.handles.ax(i),['V_{' self.compartment_names{i} '} (mV)'] )
		set(self.handles.ax(i),'YLim',[-80 80])
	end
	linkaxes(self.handles.ax,'x');
	prettyFig('plw',1.5,'lw',1);

	% make another figure for the calcium
	self.handles.fig_cal = figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 500],'CloseRequestFcn',@self.deleteManipulateFig); hold on
	n = length(self.compartment_names);
	for i = 1:n
		self.handles.ax(i) = subplot(n,1,i);
		self.handles.Ca_trace(i) = plot(self.handles.ax(i),time,Ca(:,i),'k');
		ylabel(self.handles.ax(i),['[Ca^2^+]_{' self.compartment_names{i} '} (mV)'] )
		self.handles.ax(i).YLim(1) = 0;
	end
	linkaxes(self.handles.ax,'x');
	prettyFig('plw',1.5,'lw',1);
	
	% figure out the parameters 
	% we're going to make one figure window/compartment
	is_relational = {};
	for i = 1:n
		[v,names,ir] = struct2vec(self.(self.compartment_names{i}));
		% delete all E_, m_, and h_ parameters
		rm_this = false(length(v),1);
		for j = 1:length(v)
			if strcmp(names{j}(end-1:end),'_m') || strcmp(names{j}(end-1:end),'_h') || strcmp(names{j}(end-1:end),'_E') || strcmp(names{j}(end-1:end),'_V') || any(strfind(names{j},'_Q_'))
				rm_this(j) = true;
			end
			if strcmp(names{j},'_Ca')
				rm_this(j) = true;
			end
		end
		v(rm_this) = []; names(rm_this) = []; ir(rm_this) = [];


		% remove initial underscores from names
		names = cellfun(@(x) x(2:end),names,'UniformOutput',false);

		% reconstitute into a structure
		S = struct; U = struct; L = struct;
		for j = 1:length(v)
			S.(names{j}) = v(j);
			L.(names{j}) = v(j)/10;
			U.(names{j}) = v(j)*10;
		end 
		params{i} = S; lb{i} = L; ub{i} = U; is_relational{i} = ir;
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

	% prepend temperature slider
	params = [{struct('temperature',self.temperature)} params];
	lb = [{struct('temperature',0)},lb];
	ub = [{struct('temperature',30)},ub];

	% create a puppeteer instance and configure
	p = puppeteer(params,lb,ub);

	% we're going to override pupeteer's callback functions for sliders that correspond to relational parameters, and disable those sliders
	for i = 1:length(is_relational)
		ir = is_relational{i};
		if length(p.handles) > 1
			h = p.handles(i);
		else
			h = p.handles;
		end
		for j = 1:length(h.sliders)
			if ir(j)
				h.sliders(j).Enable = 'off';
				z = strfind(h.controllabel(j).String,'=');
				h.controllabel(j).String = [h.controllabel(j).String(1:z-1) ' (relative)'] ;
				% remove the callback functions
				h.sliders(j).Callback = [];
			else

			end
		end
	end

	if self.closed_loop
		% add the puppeteer figures to xolotl's handles
		% and reconfigure their closerequests to point to xolotl's
		self.handles.pfigs = p.handles.fig;
		for i = 1:length(self.handles.pfigs)
			self.handles.pfigs(i).CloseRequestFcn = @self.deleteManipulateFig;
		end
	else
		p.attachFigure(self.handles.fig);
		p.attachFigure(self.handles.fig_cal);
	end

	if ~self.closed_loop
		p.callback_function = @self.manipulateEvaluate;
	end

	if length(self.synapses) > 0
		p.group_names = ['Temperature' self.compartment_names; 'synapses'];
	else
		p.group_names = ['Temperature' self.compartment_names];
	end


	self.handles.puppeteer_object = p;


	if self.closed_loop
        % make sure it runs realtime 
        if isfield(self.handles,'fig')
            while isvalid(self.handles.fig)
                tic

                self.updateLocalParameters(p.parameters);

                [this_V, this_Ca] = self.integrate;

                % throw out the first bit 
                V(1:buffer_chunk_size,:) = [];
                Ca(1:buffer_chunk_size,:) = [];
                
                % append the new data to the end
                V = [V; this_V];
                Ca = [Ca; this_Ca];

                for i = 1:n
                	self.handles.V_trace(i).YData = V(:,i);
                	self.handles.Ca_trace(i).YData = Ca(:,i);
                end


                drawnow limitrate
                % t = toc;
                % pause_time = (buffer_size/1e3) - t; 
                % if pause_time > 0
                %     pause(pause_time)
                % end
            end
        end
	end

	 
end % end manipulate 