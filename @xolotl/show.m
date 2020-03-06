
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
% x.show(compartment)
% x.show(conductance)
% x.show({conductance1, conductance2...})
% ```
%
% This method displays activation functions and timescales of any conductance. Subsequent calls to `show` will update the plot, plotting the new activation curves over the old ones, allowing you to compare different channels.
%
%
%
% See Also: 
% xolotl.plot
% xolotl.getGatingFunctions





function show(self,conductance, custom_name)


if isa(conductance,'cpplab')
	% we have been given a cpplab object


	if strcmp(conductance.cpp_class_name,'compartment')
		% we are being given a compartment, so we need to run this on every 
		% channel in that compartment
		channels = conductance.find('conductance');
		for i = 1:length(channels)
			self.show(conductance.(channels{i}),channels{i});
		end
		return
	end

	assert(strcmp(conductance.cpp_class_parent,'conductance'),'xolotl.show() only works for conductance-type objects')


elseif isa(conductance,'char')
	conductance = cpplab(conductance);

elseif isa(conductance,'cell')
	for i = 1:length(conductance)
		xolotl.show(conductance{i});
	end
	return
end

if strcmp(conductance.cpp_class_name,'Leak')
	return
end

% specify range of V
V = linspace(-200,200,1e3);

% build a mex-able binary using the class path
binary_name = str2func(xolotl.compileActivationFcn(conductance, self.cpp_folder));


params = conductance.get(conductance.cpp_constructor_signature);

[minf, hinf, taum, tauh] = binary_name(V, params);

if length(unique(hinf)) == 1
	tauh = NaN*tauh;
	hinf = NaN*tauh;
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
	ax(1).Tag = 'm_inf';
	ax(2).Tag = 'h_inf';
	ax(3).Tag = 'tau_m';
	ax(4).Tag = 'tau_h';

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
	custom_name = conductance.cpp_class_name;
end

plot(ax(find(strcmp({ax.Tag},'m_inf'))),V,minf,'DisplayName',custom_name);
plot(ax(find(strcmp({ax.Tag},'h_inf'))),V,hinf,'DisplayName',custom_name);
plot(ax(find(strcmp({ax.Tag},'tau_m'))),V,taum,'DisplayName',custom_name);
plot(ax(find(strcmp({ax.Tag},'tau_h'))),V,tauh,'DisplayName',custom_name);

try
	figlib.pretty();
catch
end

axes(ax(1))
legend;

% turn all YLim modes to auto
for i = 1:length(ax)
	ax(i).YLimMode = 'auto';
end
