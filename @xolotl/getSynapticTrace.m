function synaptic_trace = getSynapticTrace(self)
  % fetches the synaptic time-traces
  % for a xolotl object
  % as a cell of matrices

  % perform integration
  [V, Ca, cond_states, syn_states, ~] = self.integrate;

  % number of compartments
  n_comp = length(self.compartment_names);
  % number of synapses
  n_syns = length(self.synapses);
  % container for synaptic traces indexed by compartment
  synaptic_trace = cell(n_comp,1);

  for syndex = 1:n_syns
    for compdex = 1:n_comp
      % compare post-synaptic label with compartment label
      if strcmp(self.synapses(syndex).post,self.compartment_names{compdex})
        % append the synaptic trace to the cell
        if strcmp(self.synapses(syndex).type,'Glutamatergic')
          Esyn = -70;
        elseif strcmp(self.synapses(syndex).type,'Cholinergic')
          Esyn = -80;
        elseif strcmp(self.synapses(syndex).type,'Electrical')
          predex = find(strcmp(self.synapses(syndex).pre,self.compartment_names));
          Esyn = V(:,predex);
        else
          disp('error: cannot find the synapse type');
        end
        % compute the synaptic current
        Isyn = syn_states(:,syndex) .* ( V(:,compdex) - Esyn );
        % append the synaptic current to the proper matrix in the cell
        synaptic_trace{compdex}(:,end+1) = Isyn;
      end
    end
  end
  synaptic_trace
end
