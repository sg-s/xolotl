%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: integrate and calculate currents
%
function [current_trace, V, Ca,I_clamp, cond_state, syn_state, cont_state] = getCurrentTrace(self)
  % produces traces of the currents over time
  % for each compartment of a xolotl object

  % perform integration using pre-set parameters
  [V,Ca,~,cond_states,~]    = self.integrate;

  % number of time steps
  n_steps     = length(V(:,1));
  % number of compartment_names
  n_comps     = length(self.compartment_names);
  % number of currents, indexed by compartment
  n_currents  = zeros(n_comps,1);
  for comp_index = 1:n_comps
    % count number of conductances in each compartment
    fields    = fieldnames(self.(self.compartment_names{comp_index}));
    n_currents(comp_index)  = length(fields) - length(self.compartment_props);
  end
  % current traces, indexed by compartment
  comp_currents = cell(n_comps,1);
  curr_index_start    = 1;
  for comp_index = 1:n_comps
    % count up to twice the number of currents in a compartment
    curr_index_stop   = curr_index_start + 2*n_currents(comp_index)-1;
    % fill with n_steps x 2*n_currents matrices
    comp_currents{comp_index} = cond_states(:,curr_index_start:curr_index_stop);
    curr_index_start  = curr_index_stop + 1;
  end
  % reduce activation and inactivation variables to a single product
  for comp_index = 1:n_comps
    fields            = fieldnames(self.(self.compartment_names{comp_index}));
    IGVs              = zeros(n_steps,n_comps);
    for curr_index = 1:n_currents(comp_index)
      % odd numbers from 1 : 2*n_currents
      act_index       = 2*curr_index - 1;
      % activation variables
      act             = comp_currents{comp_index}(:,act_index);
      % inactivation variables
      ict             = comp_currents{comp_index}(:,act_index+1);
      % find ionic gating variable product
      cond_path       = self.(self.compartment_names{comp_index}).(fields{curr_index + length(self.compartment_props)}).full_path;
      cond_file       = fileread(['~/code/xolotl/' cond_path]);
      % activation
      if strfind(cond_file,'gbar*m*m*m*m') ~= []
        act_prod = act .* act .* act .* act;
      elseif strfind(cond_file,'gbar*m*m*m') ~= []
        act_prod = act .* act .* act;
      elseif strfind(cond_file,'gbar*m*m') ~= []
        act_prod = act .* act;
      elseif strfind(cond_file,'gbar*m') ~= []
        act_prod = act;
      else
        act_prod = act;
      end
      % inactivation
      if strfind(cond_file,'m*h*h*h*h') ~= []
        ict_prod = ict .* ict .* ict .* ict;
      elseif strfind(cond_file,'m*h*h*h') ~= []
        ict_prod = ict .* ict .* ict;
      elseif strfind(cond_file,'m*h*h') ~= []
        ict_prod = ict .* ict;
      elseif strfind(cond_file,'m*h') ~= []
        ict_prod = ict;
      else
        ict_prod = ict;
      end
      % activation and inactivation
      IGVs(:,curr_index) = act_prod .* ict_prod;
    end
    % fill with n_steps x n_currents matrices
    comp_currents{comp_index} = IGVs;
  end
  % scale by maximal conductances
  for comp_index      = 1:n_comps
    % identify conductances in each compartment
    fields            = fieldnames(self.(self.compartment_names{comp_index}));
    % index by field index
    for curr_index    = length(self.compartment_props)+1:length(fields)
      % keep track of current by field index
      counter         = curr_index - length(self.compartment_props);
      % keep track of electrochemical potential
      emf             = zeros(n_steps,1);
      % get maximal conductance
      gbar            = self.(self.compartment_names{comp_index}).(fields{curr_index}).gbar;
      % scale by maximal conductance
      comp_currents{comp_index}(:,counter) = comp_currents{comp_index}(:,counter)*gbar;
      % check to see if the current is calcium
      if fields{curr_index}(1:2) == 'Ca'
        emf(:) = V(:,comp_index) - Ca(:,2*comp_index);
      else
        emf(:) = V(:,comp_index) - self.(self.compartment_names{comp_index}).(fields{curr_index}).E;
      end
      % scale by electrochemical potential
      comp_currents{comp_index}(:,counter) = comp_currents{comp_index}(:,counter) .* emf;
    end
  end
  current_trace = comp_currents;

end
