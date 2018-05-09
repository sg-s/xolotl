
%%
% In this test, every single channel in the C++ code base
% will be tested, by inserting them one by one into compartments
% and stepping through them a voltage clamp protocol.
%

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
F = 96485; % Faraday constant in SI units
tau_Ca = 200;
phi = (2*f*F*vol)/tau_Ca;

% find all channels in "c++/conductance"
all_channels = getAllFiles([fileparts(fileparts(which('xolotl'))) filesep 'c++' filesep 'conductances']);

for i = 1:length(all_channels)
	[root,channel,ext] = fileparts(all_channels{i});
	if ~strcmp(ext,'.hpp')
		continue
	end
	this_channel = [pathEnd(root) filesep channel];

	cprintf('_w',[oval(i) ') ' this_channel ' '])

try
	clear x
	x = xolotl;

		x.add('AB','compartment','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',.05,'tau_Ca',tau_Ca,'Ca_target',0);

		x.AB.add(this_channel,'gbar',1e3);

		% test if voltage and current work
		[V, Ca, ~, currs] = x.integrate;

		assert(~any(isnan(currs)), 'current is NaN')

		holding_V = -60;
		all_V_step = linspace(-80,80,100);

		x.t_end = 5e2;
		x.sim_dt = .1;
		x.dt = .1;

		all_I = NaN(x.t_end/x.dt,length(all_V_step));


		x.AB.V = holding_V;
		test_I = x.integrate([],holding_V);
		x.closed_loop = false;


		if (isnan(test_I(end)))
			disp('Could not hold the cell with this channel')
			error('something went wrong')
		end

		[m_inf, h_inf] = xolotl.getGatingFunctions([pathEnd(root) filesep channel]);

		m_inf_ok = false;
		if nargin(m_inf) == 1
			m_inf_ok = ~isnan(m_inf(0));
		end

		if isnan(h_inf(0)) && m_inf_ok
			% non-inactivating channel -- attempt to fit

			for j = 1:length(all_V_step)
				all_I(:,j) = x.integrate([],all_V_step(j));
			end

			temp = x.AB.find('conductance');
			this_channel = temp{1};

			temp = all_I(end,:);
			temp = temp./(all_V_step - x.AB.(this_channel).E);

			g = temp/max(temp);


			minf = NaN*g;
			for j = 1:length(minf)
				minf(j) = m_inf(all_V_step(j));
			end


			all_n = 1:4;
			all_r2 = Inf*all_n;
			warning off
			for j = 1:4
				temp = g.^(1/j);
				rm_this = isnan(temp) | isinf(temp);
				all_r2(j) = rsquare(temp(~rm_this),minf(~rm_this));
			end
			warning on

			[maxr2,idx] = max(all_r2);
			fprintf(['[Best fit with n = ' oval(idx) ', r2 = ' oval(maxr2) ']']);

			assert(maxr2>.9,'Failed to recover m_inf curve')

		end

		cprintf('green', 'OK\n');
catch
 		cprintf('red', 'FAILED\n');
	end
end
