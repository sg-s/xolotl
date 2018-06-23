

function f = findNSpikes(V, on_off_thresh)

assert(isvector(V),'V should be a vector')
assert(~any(isnan(V)),'V cannot contain NaNs')
assert(~any(isinf(V)),'V cannot contain Infs')

if nargin < 2
	on_off_thresh = 0;
end

assert(isscalar(on_off_thresh),'on_off_thresh must be a scalar')

V = V(:);

[ons, offs] = computeOnsOffs(V > on_off_thresh);
if isempty(offs) || isempty(ons)
	f = 0;
	return
end

f = length(ons);