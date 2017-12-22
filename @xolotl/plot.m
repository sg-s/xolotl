%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: plots the activation functions of channel
% 

function ax = plot(self,cond_id,ax)
	% search for cond_id
	cond_file = [];
	for i = 1:length(self.available_conductances)
		if any(strfind(self.available_conductances{i},cond_id))
			cond_file = i;
			break;
		end
	end
	assert(~isempty(cond_file),'Which conductance do you mean?')

	lines = lineRead(self.available_conductances{cond_file});
	cond_name = pathEnd(self.available_conductances{cond_file});

	% hard code an internal calcium level 
	Ca = 0.05;

	% find m_inf 
	for i = 1:length(lines)
		if strfind(lines{i},[cond_name '::m_inf'])
			a = strfind(lines{i},'{');
			z = strfind(lines{i},'}');
			this_fragment = lines{i}(a+1:z-1);
			this_fragment = strrep(this_fragment,';','');
			this_fragment = strrep(this_fragment,'return','');
			this_fragment = strrep(this_fragment,'/','./');
			this_fragment = strrep(this_fragment,'*','.*');
			eval(['m_inf = @(V)' this_fragment ' ;'])
		end
		if strfind(lines{i},[cond_name '::h_inf'])
			a = strfind(lines{i},'{');
			z = strfind(lines{i},'}');
			this_fragment = lines{i}(a+1:z-1);
			this_fragment = strrep(this_fragment,';','');
			this_fragment = strrep(this_fragment,'return','');
			this_fragment = strrep(this_fragment,'/','./');
			this_fragment = strrep(this_fragment,'*','.*');
			eval(['h_inf = @(V)' this_fragment ' ;'])
		end
		if strfind(lines{i},[cond_name '::tau_m'])
			a = strfind(lines{i},'{');
			z = strfind(lines{i},'}');
			this_fragment = lines{i}(a+1:z-1);
			this_fragment = strrep(this_fragment,';','');
			this_fragment = strrep(this_fragment,'return','');
			this_fragment = strrep(this_fragment,'/','./');
			this_fragment = strrep(this_fragment,'*','.*');
			eval(['tau_m = @(V)' this_fragment ' ;'])
		end
		if strfind(lines{i},[cond_name '::tau_h'])
			a = strfind(lines{i},'{');
			z = strfind(lines{i},'}');
			this_fragment = lines{i}(a+1:z-1);
			this_fragment = strrep(this_fragment,';','');
			this_fragment = strrep(this_fragment,'return','');
			this_fragment = strrep(this_fragment,'/','./');
			this_fragment = strrep(this_fragment,'*','.*');
			eval(['tau_h = @(V)' this_fragment ' ;'])
		end
	end
	V = linspace(-80,80,1e3);
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
	plot(ax(1),V,m_inf(V),'DisplayName',cond_name);
	ylabel(ax(1),'m_{inf}')
	xlabel(ax(1),'V (mV)')
	try
		plot(ax(2),V,h_inf(V),'DisplayName',cond_name);
	catch
		plot(ax(2),NaN,NaN,'DisplayName',cond_name);
	end
	xlabel(ax(2),'V (mV)')
	ylabel(ax(2),'h_{inf}')

	if length(tau_m(V)) == 1
		tmV = tau_m(V) + 0*V;
	else
		tmV = tau_m(V);
	end

	plot(ax(3),V,tmV,'DisplayName',cond_name);
	ylabel(ax(3),'\tau_{m} (ms)')
	xlabel(ax(3),'V (mV)')
	set(ax(3),'YScale','log')



	try
		if length(tau_h(V)) == 1
			thV = tau_h(V) + 0*V;
		else
			thV = tau_h(V);
		end

		plot(ax(4),V,thV,'DisplayName',cond_name);
	catch
		plot(ax(4),NaN,NaN,'DisplayName',cond_name);
	end
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
	
end % end plot