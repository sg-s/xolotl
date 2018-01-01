function [act, ict, tc_act, tc_ict, mphq, cond_name] =  getGatingFunctions(self,cond_id)
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

  % initialize outputs
  % outputs
  V           = linspace(-80,80,1e3);
  act         = ones(length(V),1);
  ict         = ones(length(V),1);
  tc_act      = ones(length(V),1);
  tc_ict      = ones(length(V),1);

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
    % find h_inf
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
    % find tau_m
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
    % find tau_h
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
    if strfind(lines{i},['gbar*'])
      a = strfind(lines{i},'gbar*');
      z = strfind(lines{i},';');
      this_fragment = lines{i}(a+5:z-1);
      this_fragment = strrep(this_fragment,'*','.*');
      eval(['mphq_func = @(m,h)' this_fragment ';'])
    end
  end

  % outputs
  act         = m_inf(V);
  tc_act      = tau_m(V);
  if exist('h_inf') & exist('tau_h')
    ict         = h_inf(V);
    tc_ict      = tau_h(V);
  end
  mphq        = mphq_func(act,ict);

end
