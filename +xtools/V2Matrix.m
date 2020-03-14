%
%
% ;;     ;; ;;;;;;;;  ;;;;;;;   ;;;;;;;  ;;        ;;;;;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;       ;;
%    ;;;       ;;    ;;     ;; ;;     ;; ;;        ;;;;;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;             ;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
% ;;     ;;    ;;     ;;;;;;;   ;;;;;;;  ;;;;;;;;  ;;;;;;
%
% ### V2matrix
%
% **Syntax**
%
% ```matlab
% 	[M, V_lim, dV_lim] = xtools.V2matrix(V)
% 	[M, V_lim, dV_lim] = xtools.V2matrix(V, V_lim, dV_lim)
% ```
%
% **Description**
%
% Embeds a voltage trace in a LeMasson matrix, where `V` is a voltage time series
% as a vector, and `V_lim` and `dV_lim` are 2x1 vectors which specify the lower
% and upper bounds of `V` and its derivative.
%
% If `V_lim` and `dV_lim` are not specified, they will be computed automatically.
%
% This static method allows you to create a delay-embedding of a voltage trace,
% and then discretize the space and count the number of points in each bin.
% The resultant matrix is sometimes called a LeMasson matrix.
% `M` is the LeMasson matrix, which is always of size `101x101`.
%
% !!! info "See Also"
%     xtools.matrixCost
%     LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21
%




function [M, V_lim, dV_lim] = V2Matrix(V, V_lim, dV_lim)

corelib.assert(isvector(V),'V has to be a vector')

V = V(:);
dV = [NaN; diff(V)];

if nargin == 1
	% return V_lim and dV_lim too

	% compute V_lim
	V_lim = [min(V) - 5; max(V) + 5];

	% dV_lim
	temp = (nanmax(dV) - nanmin(dV))*.1;
	dV_lim = [min(dV) - temp, max(dV) + temp];

end

corelib.assert(isvector(V_lim),'V_lim has to be a vector')
corelib.assert(isvector(dV_lim),'dV_lim has to be a vector')
corelib.assert(length(V_lim) == 2,'size of V_lim has to be 2x1')
corelib.assert(length(dV_lim) == 2,'size of dV_lim has to be 2x1')

% overflow
V(V<V_lim(1)) = NaN;
V(V>V_lim(2)) = NaN;
dV(dV<dV_lim(1)) = NaN;
dV(dV>dV_lim(2)) = NaN;


% discetize traces
V = V - V_lim(1);
V = V/(V_lim(2) - V_lim(1));
V = ceil(V*99)+1;

dV = dV - dV_lim(1);
dV = dV/(dV_lim(2) - dV_lim(1));
dV = ceil(dV*99)+1;


M = zeros(101,101);
% M(101,101) is used to collect overflow terms

% other way
for i = 1:length(V)
	if isnan(V(i))
		M(101,101) = M(101,101) + 1;
		continue
	end
	if isnan(dV(i))
		M(101,101) = M(101,101) + 1;
		continue
	end
	M(V(i),dV(i)) = M(V(i),dV(i)) + 1;
end
