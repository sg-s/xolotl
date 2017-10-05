%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% plots the activation functions of some conductance 

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
	end
	plot(ax(1),V,m_inf(V),'DisplayName',cond_name);
	ylabel(ax(1),'m_{inf}')
	xlabel(ax(1),'V (mV)')
	try
		plot(ax(2),V,h_inf(V),'DisplayName',cond_name);
	catch
	end
	xlabel(ax(2),'V (mV)')
	ylabel(ax(2),'h_{inf}')

	plot(ax(3),V,tau_m(V),'DisplayName',cond_name);
	ylabel(ax(3),'\tau_{m} (ms)')
	xlabel(ax(3),'V (mV)')
	set(ax(3),'YScale','log')

	try
		plot(ax(4),V,tau_h(V),'DisplayName',cond_name);
	catch
	end
	ylabel(ax(4),'\tau_{h} (ms)')
	xlabel(ax(4),'V (mV)')
	set(ax(4),'YScale','log')

	prettyFig();
	
end % end plot