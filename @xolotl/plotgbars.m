%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### plotgbars


**Syntax**

```matlab
x.plotgbars('compartment_name');
x.plotgbars(axes_handle,'compartment_name');
```

**Description**

Makes a stem plot of conductance densities in a given compartment. If the first argument is a handle to a valid axis, plots will be made there. 


!!! info "See Also"
    ->xolotl.plot
    ->xolotl.show
    ->xolotl.manipulate


%}


function plotgbars(self,varargin)


if length(varargin) == 0
	error('Not enough input arguments. You need to specify which compartment to plot maximal conductances for. ')
end


if length(varargin) == 2
	axes_handle = varargin{1};
	hold(axes_handle,'on')
	compartment_name = varargin{2};
elseif length(varargin) > 0
	axes_handle = gca; hold on
	compartment_name = varargin{1};	
end



assert(isa(axes_handle,'matlab.graphics.axis.Axes'),'Expected first argument to be a axes handle')
assert(any(strcmp(self.Children,compartment_name)),'compartment_name does not resolve to a known compartment')

channel_names = self.(compartment_name).find('conductance');
g = self.(compartment_name).get('*gbar');

if ~isfield(self.handles,'gbar_plot') || ~isvalid(self.handles.gbar_plot(1))

	for j = 1:length(g)
	    self.handles.gbar_plot(j) = stem(axes_handle,j,g(j));
	end


	set(axes_handle,'XTick',1:length(g),'XTickLabel',channel_names,'XTickLabelRotation',45,'YScale','log','XLim',[.5 length(g)+.5])
	ylabel(axes_handle,'g (uS/mm^2)')


else
	% plot already exists, just update it
	for j = 1:length(g)
		self.handles.gbar_plot(j).YData = g(j);
	end
end



