%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% optimizes the parameters of a xolotl object
% using global optimization so that certain constraints
% are met
% usage:
% x.optimize({@fun1, @fun2,}, varargin)


function [varargout] = optimize(fun_list,varargin)

% check for optimisation toolbox
v = ver;
assert(any(strcmp('Optimization Toolbox', {v.Name})),'optimisation toolbox is required')
assert(any(strcmp('Global Optimization Toolbox', {v.Name})),'Global Optimization Toolbox is required')


% options and defaults
options.use_cache = true;
options.purge_cache = false;
options.use_parallel = true;
options.nsteps = 300;
options.display_type = 'iter';
options.max_fun_evals = 2e4;
options.p0 = [];
options.lb = [];
options.ub = [];
options.minimise_r2 = false;
options.engine = 'patternsearch';
options.tol_mesh = 1e-6;
options.tol_x = 1e-6;

% figure out if we should make a plot or not
options.make_plot = 0;

calling_func = dbstack;
if length(calling_func) == 1
	options.make_plot = 1;
end

if nargout && ~nargin 
	varargout{1} = options;
	return
end

% validate and accept options
if iseven(length(varargin))
	for ii = 1:2:length(varargin)-1
	temp = varargin{ii};
    if ischar(temp)
    	if ~any(find(strcmp(temp,fieldnames(options))))
    		disp(['Unknown option: ' temp])
    		disp('The allowed options are:')
    		disp(fieldnames(options))
    		error('UNKNOWN OPTION')
    	else
    		options = setfield(options,temp,varargin{ii+1});
    	end
    end
end
elseif isstruct(varargin{1})
	% should be OK...
	options = varargin{1};
else
	error('Inputs need to be name value pairs')
end

% validate inputs
if ~isa(modelname,'function_handle')
	error('First argument is not a function handle')
end
if isstruct(data)
	if any(strcmp('stimulus',fieldnames(data))) && any(strcmp('response',fieldnames(data)))
	else
		help xolotl.optimize
		error('RTFM')
	end
else
	help xolotl.optimize
	error('RTFM')
end

% hash the data and model name together 
hash = dataHash(data);
hash = dataHash([dataHash(modelname) hash]);

if options.purge_cache
	cache(hash,[])
end

% check if seed parameter structure is provided
if ~isempty(options.p0)
	p0 = options.p0;
else
	if options.use_cache
		% check the cache for p0
		p0 = cache(hash);
		if isempty(p0)
			p0 = getModelParameters(char(modelname));
		end
	else
		p0 = getModelParameters(char(modelname));
	end
end
[x0, param_names] = struct2mat(p0);
f = fieldnames(p0);
param_names = f(param_names);
default_x0 = struct2mat(p0);
		

if ~isempty(options.ub)
	ub = options.ub;
end

if ~isempty(options.lb)
	lb = options.lb;
end

% check if bounds specified
if ~exist('ub','var')
	ub = struct;
	this_ub = [];

	% intelligently ask the model what the bounds for parameters are
	mn = char(modelname);
	mn = which(mn);
	txt=fileread(mn);
	a = strfind(txt,'ub.');
	
	for i = 1:length(a)
		this_snippet = txt(a(i):length(txt));
		semicolons = strfind(this_snippet,';');
		this_snippet = this_snippet(1:semicolons(1));
		try 
			eval(this_snippet)
		catch
		end
	end

end
if isstruct(ub)
	ub_vec =  Inf*ones(length(x0),1);

	assign_these = fieldnames(ub);
	for i = 1:length(assign_these)
		assign_this = assign_these{i};
		eval(strcat('this_ub = ub.',assign_this,';'))
		ub_vec(find(strcmp(assign_this,param_names)))= this_ub;
	end

	ub = ub_vec;
end

if ~exist('lb','var')
	this_lb =[]; 
	lb = struct;

	% intelligently ask the model what the bounds for parameters are
	mn = char(modelname);
	mn = which(mn);
	txt=fileread(mn);
	a = strfind(txt,'lb.');

	for i = 1:length(a)
		this_snippet = txt(a(i):length(txt));
		semicolons = strfind(this_snippet,';');
		this_snippet = this_snippet(1:semicolons(1));
		try 
			eval(this_snippet)
		catch
		end
	end

end
if isstruct(lb)
	lb_vec = -Inf*ones(length(x0),1);

	% assign 
	assign_these = fieldnames(lb);
	for i = 1:length(assign_these)
		assign_this = assign_these{i};
		eval(strcat('this_lb = lb.',assign_this,';'))
		lb_vec(find(strcmp(assign_this,param_names)))= this_lb;
	end
	
	lb = lb_vec;

end


% pattern search options
if options.nsteps
	switch options.engine
	case 'patternsearch'
		psoptions = psoptimset('UseParallel',options.use_parallel, 'Vectorized', 'off','Cache','on','CompletePoll','on','Display',options.display_type,'MaxIter',options.nsteps,'MaxFunEvals',options.max_fun_evals,'TolMesh',options.tol_mesh,'TolX',options.tol_x);
		% search

		x = patternsearch(@(x) generalCostFunction(x,data,modelname,param_names),x0,[],[],[],[],lb,ub,psoptions);

	case 'fmincon'
		foptions = optimset('Display',options.display_type,'MaxIter',options.nsteps,'UseParallel',options.use_parallel,'MaxFunEvals',options.max_fun_evals);

		
		problem.x0 = x0;
		problem.ub = ub;
		problem.lb = lb;
		problem.solver = 'fmincon';
		problem.options = foptions;
		% search

		problem.objective = @(x) generalCostFunction(x,data,modelname,param_names);
			
		end
		x = fmincon(problem);

	end
else
	varargout{1} = p0;
	return
end


% assign outputs
p = mat2struct(x,param_names);

% save to cache only if this solution is better than the best. 
current_cost = generalCostFunction(x,data,modelname,param_names);
hash2 = dataHash(['best solution to ' hash]);


if options.purge_cache
	cache(hash2,[])
end

best_cost = cache(hash2);

if isempty(best_cost)
	% first time, cache this
	cache(hash,p);
	% also cache the cost of this solution
	cache(hash2,current_cost);
else
	% check if current cost is lower than best cost
	if current_cost > best_cost
		disp('This is not the best possible parameter set. For this dataset, these parameters:')
		disp(cache(hash))
		disp('do a better job, with a cost of:')
		disp(best_cost)
	else
		% update cache
		cache(hash,p);
		% also cache the cost of this solution
		cache(hash2,current_cost);
		disp('This is the best known parameter set for this data')
	end
	temp = fieldnames(p);
	for i = 1:length(temp)
		disp(['p.' temp{i} ' = ' oval(p.(temp{i}),4) ';'])
	end
end



if options.make_plot
	figHandles = findall(0,'Type','figure');
	make_fig = 1;
	for i = 1:length(figHandles)
		if ~isempty(figHandles(i).Tag)
			if strcmp(figHandles(i).Tag,hash)
				make_fig = 0;
				figure(figHandles(i));
				clf(figHandles(i))
			end
		end
	end
	if make_fig
		temp = figure; hold on
		set(temp,'Tag',hash)
	end
	for i = 1:length(data)
		if options.minimise_r2
			autoPlot(length(data),i,1);
			hold on
			t = 1:length(data(i).response);
			fp = modelname(data(i).stimulus,mat2struct(x,param_names));
			plot(t,data(i).response,'k')
			plot(t,fp,'r')
			% show r-square
			r2 = rsquare(fp,data(i).response);
			
			title(strcat('r^2=',oval(r2)))
			legend({'Data',char(modelname)})
		else
			autoPlot(length(data),i,1);
			hold on
			plot(data(i).response,'k')
			fp = modelname(data(i).stimulus,mat2struct(x,param_names));
			plot(fp,'r')
			% show r-square
			r2 = rsquare(fp,data(i).response);
			
			title(strcat('r^2=',oval(r2)))
			legend({'Data',char(modelname)})

			if length(unique(data(i).response)) > 2
				% fix the y scale
				ymax = 1.1*max(data(i).response(~isnan(data(i).response)));
				ymin = 0.9*min(data(i).response(~isnan(data(i).response)));
				set(gca,'YLim',[ymin ymax])
			end
		end
	end
	prettyFig('plw',1.5,'lw',1.5,'fs',14)
end

	function c =  generalCostFunction(x,data,modelname,param_names)
		if length(data) == 1
			% only fit to one data set
			if length(unique(data.response)) == 2
				% binary data, use cumsum - linear trend as proxy
				if width(data.response) > 1
					% many trials of one data set. solve for each separately
					c = 0;
					for i = 1:width(data.response)
						fp = modelname(data.stimulus(:,i),mat2struct(x,param_names));
						a = cumsum(data.response(:,i));
						a = a(:);
						a = a - linspace(a(1),a(end),length(a))';
						b = cumsum(fp); b= b(:);
						b = b - linspace(b(1),b(end),length(b))';
						c = c + cost2(a,b);
					end
				else
					a = cumsum(data.response);
					a = a(:);
					a = a - linspace(a(1),a(end),length(a))';
					b = cumsum(fp); b= b(:);
					b = b - linspace(b(1),b(end),length(b))';
					c = cost2(a,b);
				end
			else
				% normal data
				fp = modelname(data.stimulus,mat2struct(x,param_names));
				c = cost2(data.response,fp);
			
			end
			
		else
			% fit to multiple data sets at the same time
			c = NaN(length(data),1);
			w = zeros(length(data),1);
			for i = 1:length(data)
				fp = modelname(data(i).stimulus,mat2struct(x,param_names));
				c(i) = cost2(data(i).response,fp);
				w(i) = sum(~isnan(data(i).response));
				w(i) = w(i)/std(data(i).response(~isnan(data(i).response)));
			end
			% take a weighted average of the costs
			w = w/max(w);
			if ~isnan(w)
				c = mean(c.*w);
			else
				c = mean(c);
			end

		end

		if isnan(c)
			c = Inf;
		end

	end

	function c =  r2CostFunction(x,data,modelname,param_names)
		if length(data) == 1
			% only fit to one data set
			if length(unique(data.response)) == 2
				% binary data, use cumsum - linear trend as proxy
				if width(data.response) > 1
					error('This case not coded. 348')
					% many trials of one data set. solve for each separately
					c = 0;
					for i = 1:width(data.response)
						fp = modelname(data.stimulus(:,i),mat2struct(x,param_names));
						a = cumsum(data.response(:,i));
						a = a(:);
						a = a - linspace(a(1),a(end),length(a))';
						b = cumsum(fp); b= b(:);
						b = b - linspace(b(1),b(end),length(b))';
						c = c + cost2(a,b);
					end
				else
					a = cumsum(data.response);
					a = a(:);
					a = a - linspace(a(1),a(end),length(a))';
					b = cumsum(fp); b= b(:);
					b = b - linspace(b(1),b(end),length(b))';
					c = cost2(a,b);
				end
			else
				% normal data
				fp = modelname(data.stimulus,mat2struct(x,param_names));
				c = 1-abs(rsquare(data.response,fp));
			
			end
			
		else
			% fit to multiple data sets at the same time
			error('This case not coded. 377')
			c = NaN(length(data),1);
			w = zeros(length(data),1);
			for i = 1:length(data)
				fp = modelname(data(i).stimulus,mat2struct(x,param_names));
				c(i) = cost2(data(i).response,fp);
				w(i) = sum(~isnan(data(i).response));
				w(i) = w(i)/std(data(i).response(~isnan(data(i).response)));
			end
			% take a weighted average of the costs
			w = w/max(w);
			c = mean(c.*w);

		end

		if isnan(c)
			c = Inf;
		end

	end

if nargout == 1
	varargout{1} = p;
end

end % this end is for the whole function 
