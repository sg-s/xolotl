%
% ### parallelSearch
%
% looks for models using parallel workers
%
% **Syntax**
%
% ```matlab
% % assuming p is a xfind object
% p.parallelSearch()
% ```
%
% **Description**
%
% Once a xfind model is set up, the process of looking for
% models can be run on all parallel workers using 
% p.parallelSearch()
% 
% This will run in the background, and (almost immediately)
% return control to the user or script. To cancel the search
% use:
% 
% ```matlab
% cancel(p.workers)
% ```
%


function parallelSearch(self)

pool = gcp;
H = self.hash;

disp('Starting workers...')


for j = pool.NumWorkers:-1:1
	disp(['Starting worker #' mat2str(j)])

	self.workers(j) = parfeval(@self.simulate,0,[H mat2str(j) '.xfind']);	
end

