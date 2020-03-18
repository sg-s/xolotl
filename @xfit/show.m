% 
%        __ _ _   
% __  __/ _(_) |_ 
% \ \/ / |_| | __|
%  >  <|  _| | |_ 
% /_/\_\_| |_|\__|
%                 
% 
% ### show
% 
% **Syntax**
% 
% ```matlab
% xf.show()
% ```
% 
% **Description**
% 
% Assuming `xf` is a `xfit` object, and you have run the xfit
% algorithm many times and run `xf.save`, and you have been
% saving results to a `.xfit` file, then this method runs
% the user-defined `ShowFcn` and goes over all the results
% If `ShowFcn` is not defined, then it simply plots the result
% 
% See Also:
% xfit.evaluate
%     * [How to fit a xolotl model](https://xolotl.readthedocs.io/en/master/how-to/fit-parameters/)

function show(self)


if isempty(self.SaveParameters)
	disp('[ABORT] SaveParameters is empty')
	return
end



hash = hashlib.md5hash([self.SaveParameters{:}]);

% get the results
savename = [hash '.xfit'];
if exist(savename,'file') == 2
	disp('Loading saved results')
	load(savename,'-mat','SimFcnHash','cost','xolotl_hash','params')
else
	% no results saved. so we simply show what we have stored in 
	% this object

	if ~isempty(self.ShowFcn)
		self.ShowFcn(self.x, self.data)
	else
		self.x.closed_loop = true;
		self.x.integrate;
		self.x.plot()
	end
	return
end

self.SimFcnHash = hashlib.md5hash(which(func2str(self.SimFcn)),'file');


% more than one SimFcnHash, needs to be recomputed
disp('Recomputing costs...')
for i = 1:length(SimFcnHash)

	corelib.textbar(i,length(SimFcnHash))

	if SimFcnHash(i) == self.SimFcnHash
		continue
	end

	if isundefined(SimFcnHash(i))
		continue
	end

	SimFcnHash(i) = self.SimFcnHash;

	self.x.set(self.SaveParameters,params(i,:))

	cost(i) = self.evaluate(self.x.get(self.FitParameters));



end

% save it...
save(savename,'cost','SimFcnHash','-append')


[~,idx] = sort(cost,'ascend');

for i = 1:length(cost)

	if isnan(cost(i))
		continue
	end

	self.x.set(self.SaveParameters,params(idx(i),:))

	if ~isempty(self.ShowFcn)
		self.ShowFcn(self.x, self.data)
	else
		self.x.closed_loop = true;
		self.x.integrate;
		self.x.plot()
	end

	title(['Cost =  ' strlib.oval(cost(idx(i)))])


	drawnow;


	pause(.5)

end
