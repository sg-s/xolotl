%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% view a xolotl object as a graph
% needs the graph toolbox
% only shows the 

function view(self)

J = zeros(length(self.compartment_names));
E = {};
E_idx_a = [];
E_idx_z = [];

for i = 1:length(self.synapses)
	a = find(strcmp(self.synapses(i).pre,self.compartment_names));
	z = find(strcmp(self.synapses(i).post,self.compartment_names));
	if J(a,z) == 0
		J(a,z) = 1;
		E{end+1} = [self.synapses(i).type];
		E_idx_a(end+1) = a;
		E_idx_z(end+1) = z;
	else
		idx = find(E_idx_z == z .* E_idx_a == a);
		E{idx} = [E{idx} ' + ' self.synapses(i).type];
	end
end

G = digraph(J);
figure, hold on
h = plot(G,'layout','force');
h.NodeLabel = self.compartment_names;
h.MarkerSize = 10;
h.LineWidth = 3;
h.ArrowSize = 15;
h.EdgeLabel = E;