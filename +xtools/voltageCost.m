
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
% the distance between the two embeddings. The traces are first
% subsampled to N points, which speeds up the computation.
% The subsampling is handled by fast C++ accelerated code
%


function C = voltageCost(V1,V2, N, make_plot)

if nargin < 3
	N = 100;
	make_plot = false;
end

if nargin < 4
	make_plot = false;
end


corelib.assert(isvector(V1),'V1 must be a vector')
corelib.assert(isvector(V2),'V2 must be a vector')

V1 = V1(:);
V2 = V2(:);

% compute derivatives 
dV1 = [NaN; diff(V1)];
dV2 = [NaN; diff(V2)];

if nargin == 2
	N = 1e3;
end


% we need to intelligently subsample the trace. if we do something
% simple like picking one in every N points, we will miss spikes
% we need to sample in the 2D space

[small_X1, small_Y1] = statlib.uniformSample2D(dV1,V1,N);
rm_this = isnan(small_X1) | isnan(small_Y1);
small_V1 = [small_X1(~rm_this), small_Y1(~rm_this)];




% now do the same for V2
[small_X2, small_Y2] = statlib.uniformSample2D(dV2,V2,N);
rm_this = isnan(small_X2) | isnan(small_Y2);
small_V2 = [small_X2(~rm_this), small_Y2(~rm_this)];


C1 = sum(nanmin(pdist2(small_V2,small_V1)));
C1 = C1/length(small_V1);

C2 = sum(nanmin(pdist2(small_V1,small_V2)));
C2 = C2/length(small_V2);

C = C1 + C2;

if ~make_plot
	return
end

% otherwise, show the plot
figure('outerposition',[300 300 701 600],'PaperUnits','points','PaperSize',[701 600]); hold on
plot(dV2,V2,'Color',[.8 .8 1])
plot(dV1,V1,'Color',[.8 .8 .8])

plot(small_V2(:,1),small_V2(:,2),'b.')
plot(small_V1(:,1),small_V1(:,2),'k.')
