% the xtools class bundles some useful
% tools to convert measure metrics of
% spiketimes and voltage traces

classdef xtools

properties
end % end props

methods (Static)

	metrics = V2metrics(V,varargin);
	spiketimes = findNSpikeTimes(V,n_spikes,on_off_thresh);
	f = findNSpikes(V, on_off_thresh);
	[M, V_lim, dV_lim] = V2matrix(V, V_lim, dV_lim);
	C = matrixCost(M1,M2)

end % methods


end %  end classdef
