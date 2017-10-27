%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% callback for manipulate. used by puppeteer 
% when xolotl is being manipulated 

function manipulateEvaluate(self,parameters)

	self.updateLocalParameters(parameters);

	% % update all the relational parameters
	% [~,param_names,ir] = self.serialize;
	% param_names(1) = [];
	% ir(1) = [];
	
	% for i = 1:length(param_names)
	% 	% ignore E, m, h of channels
	% 	this_param_names = param_names{i};
	% 	this_ir = ir{i};
	% 	rm_this = false(length(this_param_names),1);
	% 	for j = 1:length(rm_this)
	% 		if strcmp(this_param_names{j}(end-1:end),'_m') || strcmp(this_param_names{j}(end-1:end),'_h') || strcmp(this_param_names{j}(end-1:end),'_E') || strcmp(this_param_names{j}(end-1:end),'_V') 
	% 			rm_this(j) = true;

	% 		end
	% 		if strcmp(this_param_names{j},[self.compartment_names{i} '_Ca'])
	% 			rm_this(j) = true;
	% 		end
	% 	end
	% 	this_param_names(rm_this) = [];
	% 	this_ir(rm_this) = [];
	% 	h = self.handles.puppeteer_object.handles;
	% 	if isstruct(h)
	% 	else
	% 		h = h{i};
	% 	end
	% 	for j = 1:length(this_ir)
	% 		if this_ir(j)
	% 			keyboard
	% 		end
	% 	end
	% end

	

	if ~self.closed_loop

		[V, Ca] = self.integrate;

		% update plots
		for i = 1:size(V,2)
			self.handles.V_trace(i).YData = V(:,i);
			self.handles.Ca_trace(i).YData = Ca(:,i);
		end
	end

end