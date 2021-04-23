%%
% simulate picks a new model and simulates it, and measures its metrics
% and saves it

function simulate(self, data_dump_loc)


if ~exist('data_dump_loc','var')
	H = self.x.hash(1:5);
	data_dump_loc = [ H '.xfind'];
end


f = fopen(data_dump_loc,'a+');

% make sure we close the file no matter what
finishup = onCleanup(@() fclose('all'));

while true


	% pick a new point
	self.SampleFcn();


	data = self.SimFcn();

	if self.DiscardFcn(data)
		continue

	end

	% need to log this
	self.writeToDisk(f, data);



end % infinite loop

