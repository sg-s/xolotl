function plotIV(self,cond_id,E)
  [m_inf, h_inf, tau_m, tau_h, mphq, cond_name] = getGatingFunctions(self,cond_id);
  V = linspace(-80,80,1e3);

  I = mphq .* (V - E);

  figure('outerposition',[100 100 1000 900],'PaperUnits','points','PaperSize',[1000 500]);
  plot(V,I);
  xlabel('membrane potential (mV)')
  ylabel('normalized curent (nA / \muS)')
  title([cond name ' IV Curve'])
  prettyFig();

end
