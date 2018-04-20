%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: plots the activation functions of channel
%
function ax = plot(self,conductance,ax)

[m_inf, h_inf, tau_m, tau_h] = getGatingFunctions(self,conductance);

V = linspace(-80,80,1e3);

% evaluate these functions 
minf = NaN*V;
hinf = NaN*V;
taum = NaN*V;
tauh = NaN*V;

this_compartment = strsplit(conductance,'.');
this_compartment = this_compartment{1};
Ca = self.(this_compartment).Ca_out;

for i = 1:length(V)
	if nargin(m_inf) == 1
		minf(i) = m_inf(V(i));
	else
		minf(i) = m_inf(V(i),Ca);
	end
	if nargin(h_inf) == 1
		hinf(i) = h_inf(V(i));
	else
		hinf(i) = h_inf(V(i),Ca);
	end
	
	taum(i) = tau_m(V(i));
	tauh(i) = tau_h(V(i));
end

if nargin < 3

	figure('outerposition',[100 100 1000 900],'PaperUnits','points','PaperSize',[1000 500]); hold on
	for i = 1:4
		ax(i) = subplot(2,2,i); hold on
	end
elseif any(~isvalid(ax))
	figure('outerposition',[100 100 1000 900],'PaperUnits','points','PaperSize',[1000 500]); hold on
	for i = 1:4
		ax(i) = subplot(2,2,i); hold on
	end
end


plot(ax(1),V,minf,'DisplayName',conductance);
ylabel(ax(1),'m_{inf}')
xlabel(ax(1),'V (mV)')

plot(ax(2),V,hinf,'DisplayName',conductance);
xlabel(ax(2),'V (mV)')
ylabel(ax(2),'h_{inf} (ms)')



plot(ax(3),V,taum,'DisplayName',conductance);
ylabel(ax(3),'\tau_{m} (ms)')
xlabel(ax(3),'V (mV)')
set(ax(3),'YScale','log')


plot(ax(4),V,tauh,'DisplayName',conductance);
ylabel(ax(4),'\tau_{h} (ms)')
xlabel(ax(4),'V (mV)')
set(ax(4),'YScale','log')

prettyFig();
axes(ax(1))
legend;

% turn all YLim modes to auto
for i = 1:length(ax)
	ax(i).YLimMode = 'auto';
end


