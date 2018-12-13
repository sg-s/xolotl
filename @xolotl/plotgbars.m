%{
              _       _   _ 
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

# plotgbars

makes a stem plot of conductance densities in a given compartment. 


Usage:

```
x.plotgbars('compartment_name');
x.plotgbars(axes_handle,'compartment_name');
```

### See also:

xolotl.plot
xolotl.show
xolotl.manipulate


%}


function plotgbars(self,varargin)

if length(varargin) == 2
	axes_handle = varargin{1};
	hold(axes_handle,'on')
	compartment_name = varargin{2};
else
	axes_handle = gca; hold on
	compartment_name = varargin{1};
end



assert(isa(axes_handle,'matlab.graphics.axis.Axes'),'Expected first argument to be a axes handle')
assert(any(strcmp(self.Children,compartment_name)),'compartment_name does not resolve to a known compartment')

channel_names = self.(compartment_name).find('conductance');
g = NaN(length(channel_names),1);

for i = 1:length(g)
	g(i) = self.(compartment_name).(channel_names{i}).gbar();
end


for j = 1:length(g)
    stem(axes_handle,j,g(j))
end


set(axes_handle,'XTick',1:length(g),'XTickLabel',channel_names,'XTickLabelRotation',45,'YScale','log','XLim',[.5 length(g)+.5])
ylabel(axes_handle,'g (uS/mm^2)')
