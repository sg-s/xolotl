# The xgrid class

This document describes the "xgrid" class.
This is a MATLAB class that parallelizes xolotl simulations over one or more computing clusters.
It requires the [Parallel Computing Toolbox](https://www.mathworks.com/products/parallel-computing.html) for MATLAB.

To instantiate an xgrid object, use

```matlab
p = xgrid
```

which defaults to the particle swarm optimization engine,
or specify an engine like so:

```matlab
p = xgrid('engine')
```

## Properties

Every xgrid object has the following properties.
To access any property, use dot notation, e.g.:

```matlab
p.verbosity
```

### `x`

This property contains the xolotl object that comprises the model to be simulated.
The structure of the model is important, but the exact parameter values do not,
since xgrid will spawn multiple copies of the xolotl object
and set parameter values during the parallelized simulations.

### `sim_func`

This property is a function handle to the MATLAB function is used to simulate the xolotl models.
The function must have at least one output and have the function signature

```matlab
function [outputs] = sim_func(x)
```

where `x` is the xolotl object.

There can be any number of outputs. xgrid automatically captures all outputs and saves them. 

### `n_batches`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| 10      | 1, 2, 3, ...   | double |

The user can choose how many batches of simulations each worker will perform.
A batch is a set of simulations that a worker will perform
before checking to see if there is another.

### `verbosity`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| 1      | 0, 1   | double |

Determines how much informative text xgrid will print to the command window.
When `p.verbosity == 0`, xgrid will be as quiet as possible.

### `clusters`

An internal property that keeps track of the computing clusters recruited to run xolotl simulations.

### `stagger_time`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| 1      | 1+  | double |

The amount of time (in seconds) between attempting to start a new batch of simulations.

!!! warning
    It is not advisable to reduce the stagger time below 1 second.

### `num_workers`

An internal property that keeps track of the number of workers recruited.

### `n_outputs`

An internal property that keeps track of how many outputs of the simulation function there are.

### `workers`

A protected property that lists the workers recruited.

### `n_sims`
A protected property that keeps track of how many simulations should be performed.
This is computed from the dimensionality of the input arguments to `p.batchify`.

### `xolotl_hash`

A protected property that keeps track of the MD5 hash of the xolotl object.
The hash does not change when parameters are changed,
only when the structure of the model changes
(viz. when `p.x` is modified).

### `current_pool`

A protected property for accessing the current parallel pool.

### `daemon_handle`

A protected property that consists of a vector of timers,
each of which handles a daemon on a remote cluster.

### `is_master`

| Default | Allowed Values | Type |
| ------- | -------------- | ---- |
| false      | false, true  | logical |

A protected flag that keeps track of whether this computer is the controlling computer.

### `speed`

A protected property listing the speed of simulation
as a ratio of time lapsed in the simulated world divided by the time lapsed in the real world.

### `xgrid_folder`

A hidden and protected property that lists the main directory of xgrid.

### `sim_start_time`

A hidden and protected property that keeps track of when simulations started.

## Methods


-------

### addCluster


**Syntax**

```matlab
p.addCluster('cluster_name')
```

**Description**

Adds a computer as a computing cluster to the xgrid  worker pool.
If the cluster name is `'local'`, it finds the current parallel pool on your local machine.
If the cluster name is not `'local'`, it should be an SSH address.
That computer will be recruited to run the xgrid simulation.

**Technical Details**

If the cluster name is `'local'`, then it is your local computer.
Otherwise, `xgrid` will try to ping that computer, ssh into that computer,
and set up a daemon for xgrid.
A new directory `~/.psych` will be created on that computer.



!!! info "See Also"
    * [xgrid.daemonize](../xgrid/#daemonize)
    * [xgrid.delete](../xgrid/#delete)




-------

### batchify


**Syntax**

```matlab
p.(params, param_names)
```

**Description**

This function generates a series of jobs to run on the available cluster resources.
`params` should be an M x N numerical matrix, where M is the number of parameters,
and N is the number of simulations.
`param_names` should be an M x 1 cell array of character vectors specifying xolotl properties.

Jobs are apportioned between the available cluster resources.

**Technical Details**

xgrid interprets the `param_names` as the argument to the `x.get` function,
For example, to get all maximal conductances, use
```matlab
param_names = {'*gbar'};
```



!!! info "See Also"
    * [xgrid.cleanup](../xgrid/#cleanup)
    * [xgrid.simulate](../xgrid/#simulate)
    * [xolotl.find](../xolotl/#find)



-------

### benchmark


**Syntax**

```matlab
p.benchmark()
```

**Description**

Benchmarks performance on current hardware
and saves results to `~/.psych/benchmark.mat`.



!!! info "See Also"
    * [xgrid.printLog](../xgrid/#printlog)
    * [xgrid.showWorkerStates](../xgrid/#showworkerstates)




-------

### cleanup


**Syntax**

```matlab
p.cleanup()
```

**Description**

Removes all auxiliary files generated by xgrid on all clusters
and frees all workers.

**Technical Details**

All `.ppp` files will be erased on all clusters,
and the local directory will be cleaned of `.error` files.



!!! info "See Also"
    * [xgrid.delete](../xgrid/#delete)




-------

### daemonize


**Syntax**

```matlab
p.daemonize()
```

**Description**

Sets up a daemon that listens for commands from xgrid.




!!! info "See Also"
    * [xgrid.addCluster](../xgrid/#addcluster)
    * [xgrid.stopDaemon](../xgrid/#stopdaemon)



-------

### delete


**Syntax**

```matlab
p.delete()
```

**Description**

Tries to stop the running daemons and removes the handle from the xgrid object.




!!! info "See Also"
    * [xgrid.stopDaemon](../xgrid/#stopdaemon)



-------

### gather


**Syntax**

```matlab
[all_data, all_params, all_params_idx] = p.gather()
```

**Description**

Collects together all results from all remote and local clusters.
`all_data` is a cell array where the elements are the outputs from each output `p.sim_func`.
`all_params` is an M x N matrix, where M is the number of parameters, and N is the number of simulations.
`all_params_idx` is a linear index through `all_params`.

**Technical Details**

The dimensions of `all_params` are identical to the `params` input of `batchify`.
Despite this, the matrices are not identical.
The `all_params` matrix is shuffled, due to the nature of performing the simulations in parallel.



!!! info "See Also"
    * [xgrid.batchify](../xgrid/#batchify)




-------

### getJobStatus


**Syntax**

```matlab
p.getJobStatus()
```

**Description**

Fetches the number of jobs to do, currently running jobs, and finished jobs.

**Technical Details**

This function is *internal*.



!!! info "See Also"
    * [xgrid.showWorkerStates](../xgrid/#showworkerstates)



-------

### getRemoteState


**Syntax**

```matlab
p.getRemoteState(idx)
```

**Description**

Fetches the state of a remote cluster by reading the log file.
`idx` is the index of the cluster in `p.clusters`.

**Technical Details**

This function is *internal*.



!!! info "See Also"
    * [xgrid.showWorkerStates](../xgrid/#showworkerstates)




-------

### printLog


**Syntax**

```matlab
p.printLog()
```

**Description**

Generates log files on each cluster.
The file contains the job status and state of each worker.



!!! info "See Also"
    * [xgrid.getJobStatus](../xgrid/#getjobstatus)
    * [xgrid.showWorkerStates](../xgrid/#showworkerstates)




-------

### showWorkerStates


**Syntax**

```matlab
p.showWorkerStates()
```

**Description**

Prints the state of all workers on all clusters.
Determines the state by reading the log files.

**Technical Details**

This function is *internal*.



!!! info "See Also"
    * [xgrid.simulate](../xgrid/#simulate)



-------

### simulate


**Syntax**

```matlab
p.simulate()
```

**Description**

Starts the simulation on all clusters, both local and remote.
This function should be called by the user once a simulation function is configured
and the jobs have been batched.



!!! info "See Also"
    * [xgrid.batchify](../xgrid/#batchify)
    * [xgrid.cleanup](../xgrid/#cleanup)




-------

### simulate_core


**Syntax**

```matlab
p.simulate_core(idx, n_runs)
```

**Description**

Contains the main loop that performs a job during an xgrid simulation.

**Technical Details**

This function is *internal*.
Users should call `simulate` instead.



!!! info "See Also"
    * [xgrid.simulate](../xgrid/#simulate)



-------

### startWorker


**Syntax**

```matlab
p.startWorker()
```

**Description**

SStarts a new worker.
You can use this if you want to start up one worker at a time, for whatever reason.



!!! info "See Also"
    * [xgrid.addCluster](../xgrid/#addcluster)



-------

### stop


**Syntax**

```matlab
p.stop()
```

**Description**

Stops running simulations on all clusters.

Use `delete` to stop all daemons.



!!! info "See Also"
    * [xgrid.delete](../xgrid/#delete)



-------

### stopDaemon


**Syntax**

```matlab
p.stopDaemon()
```

**Description**

Forcibly stops all running daemons. Do not use this method. 




!!! info "See Also"
    * [xgrid.delete](../xgrid/#delete)



-------

### tellRemote


**Syntax**

```matlab
p.tellRemote(cluster_name, command, value)
```

**Description**

Do not use this method. 




!!! info "See Also"
    * [xgrid.addCluster](../xgrid/#addcluster)




-------

### unpack


**Syntax**

```matlab
p.unpack(all_data)
```

**Description**

Unpacks data from `all_data` and turns it into variables,
as defined in `p.sim_func`.





-------

### wait


**Syntax**

```matlab
p.wait()
```

**Description**

Waits for all simulations to be finished on all clusters.



!!! info "See Also"
    * [xgrid.simulate](../xgrid/#simulate)
    * [xgrid.stop](../xgrid/#stop)



-------

### xgridd


**Syntax**

```matlab
p.xgridd(~, ~)
```

**Description**

This is the daemon version of xgrid.
It's a very simple loop that is meant to be run on a timer.
Every time it runs, it looks to see if there is a command that tells it to do something,
and if so, tries to do it.

**Technical Details**

This function should *never* throw an error,
so count on it to be running at all times.


