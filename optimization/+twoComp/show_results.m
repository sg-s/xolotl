% this script shows the best solutions we found
function [x, parameter_names, parameters] = show_results()

p = twoComp.setup;
data = twoComp.getData;


d = fileparts(fileparts(mfilename('fullpath')));

% load data results
load([d filesep 'ed7f78fe051e19853bfeb427a3e2943f.xfit'],'-mat','cost','params');


% remove duplicates
[params, idx] = unique(params,'rows');



% show some passing ones and throw in the real ones for sport

show_these = [714 579 967 244 1173 760 1161 115 765 445 775 1177 635 945 51 886 837 407 802 413 924 193 1072];

figure('outerposition',[300 300 801 1100],'PaperUnits','points','PaperSize',[801 1100]); hold on

real_data = veclib.shuffle(1:18);
real_data = real_data(1:4);
disp(real_data)
idx = 1;

% add some noise
p.x.CellBody.add('CurrentNoise','noise_amplitude',.5);

for i = 1:18
	subplot(6,3,i); hold on

	if ismember(i,real_data)
		time = (1:length(data(idx).V0))*1e-4;
		plot(time,data(idx).V0,'k')
		set(gca,'XLim',[0 5])
		idx = idx + 1;
	else

		p.x.set(p.parameter_names,params(show_these(i),:))
		p.x.closed_loop = true;
		p.x.reset;
		p.x.integrate;
		p.x.integrate;
		V = p.x.integrate;
		time = (1:length(V))*p.x.dt*1e-3;
		plot(time,V(:,2)+ randn(length(V),1)*5e-2,'k')

		%title([mat2str(show_these(i)) '  cost = ' strlib.oval(cost(show_these(i)))])
	end
	axis off
end

figlib.pretty('PlotLineWidth',1)


x = p.x;
parameter_names = p.parameter_names;

parameters = params(show_these,:);


