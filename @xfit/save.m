% 
%        __ _ _   
% __  __/ _(_) |_ 
% \ \/ / |_| | __|
%  >  <|  _| | |_ 
% /_/\_\_| |_|\__|
%                 
% 
% ### save
% 
% **Syntax**
% 
% ```matlab
% xf.save()
% ```
% 
% **Description**
% 
% Assuming `xf` is a `xfit` object, save results of 
% this optimization run in a database called <hash>.xfit
% where <hash> is the hash of parameters that will be saved
% 
% 
% See Also:
% xfit.fit
%     * [How to fit a xolotl model](https://xolotl.readthedocs.io/en/master/how-to/fit-parameters/)

function save(self)

if isempty(self.SaveParameters)
	return
end

this_cost = self.evaluate(self.seed);

if this_cost > self.SaveWhenCostBelow
	return
end

hash = hashlib.md5hash([self.SaveParameters{:}]);

% check if there is file with this name
savename = [pwd filesep hash '.xfit'];
if exist(savename,'File') == 2
	load(savename, 'xolotl_hash','SimFcnHash','cost','params','-mat')
	idx = find(isundefined(xolotl_hash),1,'first');

else
	% no file, let's create a new one 
	xolotl_hash = categorical(NaN(100,1));
	SimFcnHash = categorical(NaN(100,1));


	cost = NaN(100,1);
	params = NaN(100,length(self.SaveParameters));

	idx = 1;

end

if isempty(idx)
	% we need to extend the arrays
	idx = length(xolotl_hash) + 100;
end


xolotl_hash(idx) = self.x.hash;
SimFcnHash(idx) = self.SimFcnHash;
cost(idx) = this_cost;
params(idx,:) = self.x.get(self.SaveParameters);


save(savename, 'xolotl_hash','SimFcnHash','cost','params')

disp('Saved this parameter set to disk')