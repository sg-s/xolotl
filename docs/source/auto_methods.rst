.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _methods:

Methods
=======

This page lists the methods of the ``xolotl`` class in ``MATLAB``. This can serve as a reference for advanced usage. 

In the rest of this documentation we will assume a ``xolotl`` object named ``x`` that can be created using

  x = xolotl;

.. hint::

  You can list all the methods of ``xolotl`` object ``x`` by ::

    x.methods;
.. _add:

add
^^^

adds a ``cpplab`` object to a ``xolotl`` object.

The add method is the most important way you construct models. Usage ::

	x.add(compartment,'comp_name')
	x.add('compartment','comp_name')
	x.add('compartment','comp_name',...)

There are two primary ways of using ``add``. The first is to first construct a ``cpplab`` object (here called AB), and then add it to the ``xolotl`` object using ``x.add(AB,'AB')``. ``xolotl`` requires that every compartment is named, and the name has to be specified as a string argument. 

.. warning::

Some read-only properties in a xolotl object may not be copied over. 





Test coverage
--------------

``add`` is tested in: 

`test_bursting_neuron.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_bursting_neuron.m>_
`test_clamp.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_clamp.m>_
`test_fI.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_fI.m>_
`test_integral_control.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_integral_control.m>_
`test_stg.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_stg.m>_
`test_stg_temperature.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_stg_temperature.m>_


.. _benchmark:

 benchmark
 ^^^^^^^^^

 performs a quick benchmarking of a given ``xolotl`` model. ``benchmark`` first varies the simulation time step, and measures how quickly the model integrates. It then varies ``t_end``, and measures how fast it integrates at a fixed ``sim_dt``. Usage ::

   x.benchmark;




Test coverage
--------------

``benchmark`` is tested in: 



.. _checkCompartmentName:

checkCompartmentName
^^^^^^^^^^^^^^^^^^^^

is used internally by ``xolotl`` to verify that the compartment name you are using is valid and legal. This method is called every time you add a compartment to a ``xolotl`` object. Usage ::

   ok = checkCompartmentName(self,comp_name)





Test coverage
--------------

``checkCompartmentName`` is tested in: 



.. _cleanup:

cleanup
^^^^^^^
A static method that cleans up all transpiled ``C++`` and compiled binary files. Usage ::

   xolotl.cleanup
   x.cleanup

Use of this method will trigger a warning every time it is called. You do not need to use this in normal use, but can call this to force a recompile, or to delete old and unused binaries. 




Test coverage
--------------

``cleanup`` is tested in: 

`run_all_tests.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/run_all_tests.m>_


.. _compile:

 compile
 ^^^^^^^

 compiles a executable binary form a transpiled ``C++`` file. These are stored in your ``xolotl`` directory. ``xolotl`` automatically compiles when it needs to. You can turn this functionality off by setting ::

   x.skip_hash = true;

 In addition, creating a ``xolotl`` object through a function call does not automatically hash and compile. In this case, you should use ``x.md5hash``.

 .. warning::

   If you turn hashing off, ``xolotl`` might not compile




Test coverage
--------------

``compile`` is tested in: 



.. _contributingCurrents:

 contributingCurrents
 ^^^^^^^^^^^^^^^^^^^^

 calculates the contributions of each current at every point in a voltage trace. This is used internally in ``xolotl.plot`` to color voltage traces. Syntax ::

   curr_index = contributingCurrents(V, I)

 where V is a vector of voltages, I is the corresponding matrix of currents %



Test coverage
--------------

``contributingCurrents`` is tested in: 



.. _copy:

 copy
 ^^^^

 copies a xolotl object. ``copy`` creates an identical copy of a xolotl object that can be manipulated seperately. Both copies will use the same binary to integrate, unless you add a new component to one of them. Syntax ::

   x2 = copy(x);

 .. warning::

   Some read-only properties in a xolotl object may not be copied over. 



Test coverage
--------------

``copy`` is tested in: 



.. _findNSpikes:

 findNSpikes
 ^^^^^^^^^^^

 static method of ``xolotl`` that computes the number of spikes in a voltage trace. Example use ::

   f = findNSpikes(V);
   f = findNSpikes(V, on_off_thresh)

 ``V`` is a vector of voltages, and ``on_off_thresh`` is an optional argument that determines the threshold above which a voltage fluctuation is considered a spikes. The default is 0. 





Test coverage
--------------

``findNSpikes`` is tested in: 



.. _manipulate:

manipulate
^^^^^^^^^^

method that allows you to manipulate some or all parameters in a model hile visualizing its behaviour. Usage ::

   x.manipulate();
   x.manipulate('some*pattern')
   x.manipulate({'parameter1','parameter2'})

The simplest way to use ``manipulate`` is to simply call it with no arguments. By default, all the parameters are linked to sliders that you can play with. In models with a large number of parameters, this can get messy. You can selectively only manipualte some parameters whose names match a pattern using ``x.manipulate('some*pattern')``






function manipulate(self, manipulate_these)


if isempty(self.linked_binary)
	self.transpile;
	self.compile;
end

 disable closed loop
self.closed_loop = false;

[V,Ca] = self.integrate;
time = (1:length(V))*self.dt*1e-3;

t_end = self.t_end;

compartment_names = self.find('compartment');
n = length(compartment_names);




if nargin < 2

	[values, ~, ~, real_names] = self.serialize;

	% skip some dynamical values
	rm_this = [lineFind(real_names,'*dt'); lineFind(real_names,'*.m'); lineFind(real_names,'*.h'); lineFind(real_names,'synapses*.s')];

	% manually remove all the V, Ca for each neuron
	for i = 1:length(real_names)
		for j = 1:n
			if strcmp(real_names{i}, [compartment_names{j} '.Ca'])
				rm_this = [rm_this; i];
			end
			if strcmp(real_names{i}, [compartment_names{j} '.V'])
				rm_this = [rm_this; i];
			end
		end
	end

	values(rm_this) = [];
	real_names(rm_this) = [];

else

	if ~iscell(manipulate_these) && any(strfind(manipulate_these,'*'))
		% first find objects, then get them
		manipulate_these = self.find(manipulate_these);
	end

	real_names = manipulate_these;
	values = NaN*ones(length(real_names),1);
	% check that these exist
	for i = 1:length(real_names)
		assert(self.exist(real_names{i}),'Unknown parameter to manipulate')
		temp = self.get(real_names{i});
		if isa(temp,'function_handle')
			temp = temp();
		end
		values(i) = temp;
	end
end

assert(~isempty(manipulate_these),'Manipulate was called with illegal or invalid parameters that did not resolve to anything.')

self.manipulate_plot_func{1}(self);


 semi-intelligently make the upper and lower bounds
lb = values/3;
ub = values*3;


warning('off','MATLAB:hg:uicontrol:ValueMustBeInRange')

 create a puppeteer instance and configure
p = puppeteer(real_names,values,lb,ub,[],true);



p.attachFigure(self.handles.fig);


Test coverage
--------------

``manipulate`` is tested in: 

`custom_fI.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/custom_fI.m>_
`test_fI.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_fI.m>_


.. _:



Test coverage
--------------

```` is tested in: 

`custom_fI.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/custom_fI.m>_
`run_all_tests.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/run_all_tests.m>_
`test_bursting_neuron.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_bursting_neuron.m>_
`test_clamp.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_clamp.m>_
`test_fI.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_fI.m>_
`test_integral_control.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_integral_control.m>_
`test_stg.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_stg.m>_
`test_stg_temperature.m <https://https://github.com/sg-s/xolotl/blob/master/%40xolotl/test_stg_temperature.m>_


