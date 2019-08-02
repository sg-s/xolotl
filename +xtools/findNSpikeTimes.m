
%
% ;;     ;; ;;;;;;;;  ;;;;;;;   ;;;;;;;  ;;        ;;;;;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;       ;;
%    ;;;       ;;    ;;     ;; ;;     ;; ;;        ;;;;;;
%   ;; ;;      ;;    ;;     ;; ;;     ;; ;;             ;;
%  ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
% ;;     ;;    ;;     ;;;;;;;   ;;;;;;;  ;;;;;;;;  ;;;;;;
%
% ### findNSpikeTimes
%
%
%
%
%
% **Syntax**
%
% ```matlab
% spike_times = xtools.findNSpikeTimes(V, n_spikes, on_off_thresh)
% ```
%
% **Description**
%
% Computes the number of spikes in a voltage trace. `V` is an n x 1 voltage trace
% Spikes are defined as voltage crossings across a threshold, `on_off_thresh` (default = 0 mV).
%
% !!! info "See Also"
%     xtools.findNSpikeTimes
%
%


function spike_times = findNSpikeTimes(V,n_spikes,on_off_thresh)

if nargin < 3
	on_off_thresh = 0;
end

spike_times = NaN(n_spikes,1);
[ons, offs] = veclib.computeOnsOffs(V > on_off_thresh);
if isempty(offs) || isempty(ons)
	return
end

stop_here = min([length(ons) n_spikes]);

for j = 1:stop_here
  [~,idx] = max(V(ons(j):offs(j)));
  spike_times(j) = ons(j) + idx;
end
