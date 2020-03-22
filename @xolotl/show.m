
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### show
%
% **Syntax**
%
% ```matlab
% x.show('path/to/conductance/file')
% x.show(x.Compartment)
% x.show(x.Compartment.Conductance)
% x.show({conductance1, conductance2...})
% x.show(x.Compartment.Synapse)
% ```
%
% This method displays activation functions and timescales of any conductance or synapse. If you are 
% passing a conductance argument, subsequent calls to `show` will update the plot, 
% plotting the new activation curves over the old ones, allowing you to compare different channels.
%
%
%
% See Also: 
% xolotl.plot
% xolotl.getGatingFunctions





function show(self,obj, custom_name)


if isa(obj,'cpplab')
	% we have been given a cpplab object


	if strcmp(obj.cpp_class_name,'compartment')
		% we are being given a compartment, so we need to run this on every 
		% channel in that compartment
		channels = obj.find('conductance');
		for i = 1:length(channels)
			self.show(obj.(channels{i}),channels{i});
		end
		return
	end

	corelib.assert(strcmp(obj.cpp_class_parent,'conductance') || strcmp(obj.cpp_class_parent,'synapse'),'xolotl.show() only works for conductance-type or synapse-type objects')


elseif isa(obj,'char')
	obj = cpplab(obj);

elseif isa(obj,'cell')
	for i = 1:length(obj)
		xolotl.show(obj{i});
	end
	return
end

if strcmp(obj.cpp_class_name,'Leak')
	return
end

% specify range of V
V = linspace(-200,200,1e3);

% build a mex-able binary using the class path
if strcmp(obj.cpp_class_parent,'synapse')
	binary_name = str2func(xolotl.compileSynapseFcn(obj, self.cpp_folder));
	params = obj.get(obj.cpp_constructor_signature);

else
	binary_name = str2func(xolotl.compileActivationFcn(obj, self.cpp_folder));


	params = obj.get(obj.cpp_constructor_signature);

	[data.minf, data.hinf, data.taum, data.tauh] = binary_name(V, params);

	if length(unique(data.hinf)) == 1
		data.tauh = NaN*data.tauh;
		data.hinf = NaN*data.tauh;
	end

end




if strcmp(obj.cpp_class_parent,'synapse')


	V_base = -80;
	V_step = -80:1:50;




	figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
	ax.sinf = subplot(1,2,1); hold on
	xlabel('V (mV)')
	ylabel('s_{\infty} ')

	ax.taus = subplot(1,2,2); hold on
	xlabel('V (mV)')
	ylabel('Approximate \tau_{s} (ms)')

	sinf = NaN*V_step;
	taus = NaN*V_step;

	for i = 1:length(V_step)
		V = zeros(1e4,1) + V_step(i);
		V(1:2e3) = V_base;

		V = V(:);

		s = binary_name(V,params);

		sinf(i) = s(end);

		taus(i) = find(s>s(end)/exp(1),1,'first')/10; % ms

	end

	l  =plot(ax.sinf,V_step,sinf,'k','DisplayName',obj.cpp_class_name);
	plot(ax.taus,V_step,taus,'k')

	legend(l);


	figlib.pretty()

	return

end




% check if there is any figure that we can plot on
allfigs = get(0, 'Children');
f = [];
for i = 1:length(allfigs)
	if ~isempty(allfigs(i).Tag) && strcmp(allfigs(i).Tag,'xolotl-show')
		f = allfigs(i);
		ax  = findall(f,'type','axes');

	end
end

if isempty(f)

	f = figure('outerposition',[100 100 1000 900],'PaperUnits','points','PaperSize',[1000 500]); hold on
	for i = 1:4
		ax(i) = subplot(2,2,i); hold on
	end
	ax(1).Tag = 'minf';
	ax(2).Tag = 'hinf';
	ax(3).Tag = 'taum';
	ax(4).Tag = 'tauh';

	ylabel(ax(1),'m_{\infty}')
	xlabel(ax(1),'V (mV)')
	set(ax(1),'YLim',[0 1],'XLim',[-100 100])

	xlabel(ax(2),'V (mV)')
	ylabel(ax(2),'h_{\infty}')
	set(ax(2),'YLim',[0 1],'XLim',[-100 100])

	ylabel(ax(3),'\tau_{m} (ms)')
	xlabel(ax(3),'V (mV)')
	set(ax(3),'YScale','log','XLim',[-100 100])

	ylabel(ax(4),'\tau_{h} (ms)')
	xlabel(ax(4),'V (mV)')
	set(ax(4),'YScale','log','XLim',[-100 100])

	f.Tag = 'xolotl-show';
end



if nargin < 3
	custom_name = obj.cpp_class_name;
end


% should we make a new plot, or just move the old one?
fn = fieldnames(data);
if isempty(ax(1).Children)
	plot(ax(find(strcmp({ax.Tag},'minf'))),V,data.minf,'DisplayName',custom_name,'Tag',obj.hash);
	plot(ax(find(strcmp({ax.Tag},'hinf'))),V,data.hinf,'DisplayName',custom_name,'Tag',obj.hash);
	plot(ax(find(strcmp({ax.Tag},'taum'))),V,data.taum,'DisplayName',custom_name,'Tag',obj.hash);
	plot(ax(find(strcmp({ax.Tag},'tauh'))),V,data.tauh,'DisplayName',custom_name,'Tag',obj.hash);
else
	% there are pre-existing plots. Make sure we don't plot the same object to two different plots
	for i = 1:length(ax)
		old_plots = ax(i).Children;
		plothere = find(strcmp({old_plots.Tag},obj.hash));
		if isempty(plothere)
			% make new plot
			plot(ax(i),V,data.(fn{i}),'DisplayName',custom_name,'Tag',obj.hash);
		else
			% change YData in this plot
			plothere = ax(i).Children(plothere);
			plothere.YData = data.(fn{find(strcmp(fn,ax(i).Tag))});
		end
	end
end

figlib.pretty();

axes(ax(1))
legend;

% turn all YLim modes to auto
for i = 1:length(ax)
	ax(i).YLimMode = 'auto';
end
