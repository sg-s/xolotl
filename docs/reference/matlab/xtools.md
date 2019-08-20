# The xtools package



This document describes the `xtools` package. `xtools` is [MATLAB package](https://www.mathworks.com/help/matlab/matlab_oop/scoping-classes-with-packages.html) that contains usefull tools that are stand-alone functions. Since they exist inside a special folder that begins with a `+`, MATLAB knows that it is a "package", and you can access any function using dot notation:

```matlab
xtools.FunctionName()
```


## Functions

-------

### V2metrics

**Syntax**

```matlab
options = xtools.V2metrics
metrics = xtools.V2metrics(V)
metrics = xtools.V2metrics(V, options)
metrics = xtools.V2metrics(V, 'PropertyName', PropertyValue, ...)
```

**Description**

Computes metrics from a raw time series of voltage,
which can be experimental or simulated data.

If called without arguments or outputs, a struct
containing fields for all optional arguments, `options`,
is created.

`V2metrics` can be called using a struct to specify
options, or with individual options specified as name, value keyword pairs. Options with a `NaN` value are ignored
and the default option value is used instead.

| Option Name | Default Value | Units |
| ----------- | ------------- | ----- |
| `sampling_rate` | 20 | 1/ms |
| `ibi_thresh` | 300 | ms |
| `spike_threshold` | 0 | mV |
| `debug` | false | |

| Metric Name | Default Value | Units |
| ----------- | ----- | ----- |
| `firing_rate` | NaN | ? |
| `burst_period` | NaN | ? |
| `ibi_mean` | NaN | ? |
| `ibi_std` | NaN | ? |
| `isi_std` | NaN | ? |
| `burst_period_std` | NaN | ? |
| `isi_std` | NaN | ? |
| `duty_cycle_mean` | NaN | ? |
| `n_spikes_per_burst_mean` | NaN | ? |
| `n_spikes_per_burst_std` | NaN | ? |
| `min_V_mean` | NaN | ? |
| `min_V_std` | NaN | ? |
| `min_V_in_burst_mean` | NaN | ? |
| `min_V_in_burst_std` | NaN | ? |
| `spike_peak_mean`  | NaN | ? |
| `spike_peak_std` | NaN | ? |




!!! info "See Also"
    * [xtools.findNSpikes](../xtools/#findnspikes)
    * [xtools.findNSpikeTimes](../xtools/#findnspiketimes)
    * [xtools.findBurstMetrics](../xtools/#findburstmetrics)
    * [LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21](../LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting/#in: de schutter e (ed) computational neuroscience: realistic modeling for experimentalists)






-------

### binCost

**Syntax**

```matlab
c = xfit.binCost(allowed_range, actual_value)
```

**Description**

A static method that computes a quadratic cost
when `actual_value` is outside of the minimum and maximum
set by the 2-vector `allowed_range`, and returns zero otherwise.

This method can be used as part of a simulation function
when it is important for neurocomputational properties of interest to
fit within a given range, rather than necessarily fit a value perfectly.



-------

### findBurstMetrics





**Syntax**

```matlab
xtools.findBurstMetrics(V, Ca)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca, Ca_peak_similarity)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca, Ca_peak_similarity, burst_duration_variability)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca, Ca_peak_similarity, burst_duration_variability, on_off_thresh)
```

**Description**

Computes burst metrics, spike times, and peaks and troughs of calcium wave.

**Arguments**

`V` is an n x 1 vector describing the membrane potential of a compartment over time.

`Ca` is an n x 1 vector describing the intracellular calcium concentration of the
same compartment over time.

| Optional Positional Argument | Default Value |
| --------  | ------ |
| `Ca_peak_similarity` | 0.3
| `burst_duration_variability` | 0.1 |
| `on_off_thresh` | 0 |

`Ca_peak_similarity` sets the maximum acceptable coefficient of variation (CV) between
the intracellular calcium peaks. This function will exit with an error code if this limit is
exceeded. To ignore this limit, set it to `Inf`.

`burst_duration_variability` sets the maximum acceptable CV between the burst periods.
This function will exit with an error code if this limit is exceeded. To ignore this
limit, set it to `Inf`.

`on_off_thresh` determines the horizontal crossing line (in units of mV) at which
spikes should be counted. For example, if the `on_off_thresh` is set to 10 mV, then
only when the membrane potential crosses 10 mV will a spike be counted.

**Outputs**

With no outputs, the burst period, mean number of spikes per burst, and duty cycle
are printed to the command window.

`burst-metrics` is a 10x1 vector which contains the

1.  burst period (in units of dt)
2.  mean number of spikes per burst (unitless)
3.  time of first spike within a burst relative to  the burst's Ca peak (in units of dt)
4.  time of last spike within a burst relative to  the burst's Ca peak (in units of dt)
5.  mean of calcium peaks ($\mu M$)
6.  mean of Calcium troughs ($\mu M$)
7.  variability of Calcium peaks (coefficient of variation)
8.  variability of burst periods (coefficient of variation)
9.  duty cycle (unitless)
10. error code

The error code is a digit between 0 and 4 that describes the success of the function.

0. 	No error
1.	Fewer than 5 Calcium peaks
2. 	Calcium peaks not similar enough
3. 	Burst periods too variable
4. 	No spikes

`spike_times` is an n x 1 vector containing the times (in units of dt) of each spike
in the trace defined by V.

`Ca_peaks` is an n x 1 vector containing the times (in units of dt) of each peak
of the calcium wave.

`Ca_troughs` is an n x 1 vector containing the times (in units of dt) of each trough
of the calcium wave.

!!! info "Dependencies"
    This function requires the Signal Processing Toolbox for MATLAB.





-------

### findNSpikeTimes





**Syntax**

```matlab
spike_times = xtools.findNSpikeTimes(V, n_spikes, on_off_thresh)
```

**Description**

Computes the number of spikes in a voltage trace. `V` is an n x 1 voltage trace
Spikes are defined as voltage crossings across a threshold, `on_off_thresh` (default = 0 mV).



!!! info "See Also"
    * [xtools.findNSpikeTimes](../xtools/#findnspiketimes)






-------

### findNSpikes





**Syntax**

```matlab
N = xtools.findNSpikes(V);
N = xtools.findNSpikes(V, on_off_thresh)
```

**Description**

Computes the number of spikes in a voltage trace. `V` is an n x 1 voltage trace
and `on_off_thresh` is a membrane potential threshhold at which spikes should be
counted (default = 0 mV).



!!! info "See Also"
    * [xtools.findNSpikeTimes](../xtools/#findnspiketimes)






-------

### matrixCost

**Syntax**

```matlab
C = xtools.matrixCost(M1, M2)
```

**Description**

Compute the norm-squared distance between two matrices. If the matrices are LeMasson
matrices, which represent discretized probability distributions of a derivative-embedded
attractor of a voltage trace, then this distance serves as a measure of how dissimilar
the two voltage traces are.



!!! info "See Also"
    * [xtools.V2Matrix](../xtools/#v2matrix)
    * [LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21](../LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting/#in: de schutter e (ed) computational neuroscience: realistic modeling for experimentalists)







-------

### voltageCost

**Syntax**

```matlab
C = xtools.voltageCost(V1, V2, N)
```

**Description**

embeds two voltage traces w.r.t to their derivatives, and measures
the distance between the two embeddings. The traces are first
subsampled to N points, which speeds up the computation.
The subsampling is handled by fast C++ accelerated code



