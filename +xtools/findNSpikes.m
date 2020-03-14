
%
% ;;     ;; ;;;;;;;;  ;;;;;;;   ;;;;;;;  ;;        ;;;;;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;       ;;
%    ;;;       ;;    ;;     ;; ;;     ;; ;;        ;;;;;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;             ;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
% ;;     ;;    ;;     ;;;;;;;   ;;;;;;;  ;;;;;;;;  ;;;;;;
%
% ### findNSpikes
%
%
%
%
%
% **Syntax**
%
% ```matlab
% N = xtools.findNSpikes(V);
% N = xtools.findNSpikes(V, on_off_thresh)
% ```
%
% **Description**
%
% Computes the number of spikes in a voltage trace. `V` is an n x 1 voltage trace
% and `on_off_thresh` is a membrane potential threshhold at which spikes should be
% counted (default = 0 mV).
%
% !!! info "See Also"
%     xtools.findNSpikeTimes
%
%


function f = findNSpikes(V, on_off_thresh)

corelib.assert(isvector(V),'V should be a vector')
corelib.assert(~any(isnan(V)),'V cannot contain NaNs')
corelib.assert(~any(isinf(V)),'V cannot contain Infs')

if nargin < 2
	on_off_thresh = 0;
end

corelib.assert(isscalar(on_off_thresh),'on_off_thresh must be a scalar')

V = V(:);

[ons, offs] = veclib.computeOnsOffs(V > on_off_thresh);
if isempty(offs) || isempty(ons)
	f = 0;
	return
end

f = min([length(ons) length(offs)]);
