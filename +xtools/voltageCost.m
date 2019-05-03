
% ### voltageCost
%
% **Syntax**
%
% ```matlab
% 	C = xtools.voltageCost(V1, V2, N)
% ```
%
% **Description**
%
% embeds two voltage traces w.r.t to their derivatives, and measures
% the distance between the two embeddings
%


function C = voltageCost(V1,V2, N)

C = 0;


% compute derivatives 
dV1 = [NaN; diff(V1)];
dV2 = [NaN; diff(V2)];

assert(isvector(V1),'V1 must be a vector')
assert(isvector(V2),'V2 must be a vector')

if nargin == 2
	N = 1e3;
end


% we need to inteligently subsample the trace. if we do something
% simple like picking one in every N points, we will miss spikes
% we need to sample in the 2D space
x_space =  linspace(nanmin(dV1),nanmax(dV1),N);

% pick one point at random for each step along the x_space
small_V1 = [NaN*x_space; NaN*x_space];
for i = 2:length(x_space)
	allowed_values = find(dV1 > x_space(i-1) & dV1 < x_space(i));

	if isempty(allowed_values)
		continue
	end

	small_V1(1,i) = dV1(allowed_values(1));
	small_V1(2,i) = V1(dV1 == small_V1(1,i));
end



% now do the same for V2
x_space =  linspace(nanmin(dV2),nanmax(dV2),N);

% pick one point at random for each step along the x_space
small_V2 = [NaN*x_space; NaN*x_space];
for i = 2:length(x_space)
	allowed_values = find(dV2 > x_space(i-1) & dV2 < x_space(i));

	if isempty(allowed_values)
		continue
	end

	small_V2(1,i) = dV2(allowed_values(1));
	small_V2(2,i) = V2(dV2 == small_V2(1,i));
end


small_V1(:,isnan(small_V1(1,:))) = [];
small_V2(:,isnan(small_V2(1,:))) = [];

small_V1 = small_V1';
small_V2 = small_V2';

for i = 1:length(small_V1)
	C = C + nanmin(pdist2(small_V2,small_V1(i,:)));

end

C = C/length(small_V1);