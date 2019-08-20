


## Who wrote xolotl?

``xolotl`` was designed and written by [Srinivas Gorur-Shandilya](https://srinivas.gs/) and Alec Hoyland in
[Eve Marder's laboratory](https://blogs.brandeis.edu/marderlab/) at Brandeis University.

## What's with the name? And how do I pronounce it?

Xolotl is the Mesoamerican god of [lightning and death](https://en.wikipedia.org/wiki/Xolotl). It is the humble opinion of this writer that "xolotl" is a better name for a neuron simulator than say, "NEURON".

[Click here to listen to a pronunciation.](https://upload.wikimedia.org/wikipedia/commons/4/48/Xolotl.ogg)



## How can I contribute?

``xolotl`` is far from complete and contributions are welcome. Check out our [guide](contributing.md)
on how to report bugs, add conductances and mechanisms, or contribute to the code base.

## Something is broken, what do I do?

As the Hitchiker's Guide to the Galaxy reads, "DON'T PANIC".
The most common error we see is an issue between ``MATLAB``,
the ``MEX`` compiler, and your computer. In that
situation, a first step would be to check our [compilers](how-to/install-configure.md) guide which has detailed fixes for
some of the most common compiler issues we've seen.


If it's not a compiler issue and you've checked all the typical programming
pitfalls (syntax, spelling, your ``MATLAB`` path, cleared your workspace, etc...),
try the following

```
% erases all the compiled binaries of xolotl networks
xolotl.cleanup

% rebuilds the cache of C++ objects
cpplab.rebuildCache
```

If you're still having a problem, we might have fixed it already! Reinstall or update by running the `xolotl.update()` if you installed the toolbox or pulling from the repository if you're using `git`.
Remember that if you're using `git` you should be sure to pull all the dependencies as well
(`srinivas.gs_mtools`, `cpplab`, `puppeteer`).

You can also check our [troubleshooting](troubleshooting.md) page for known fixes to common problems.


## How do I cite this?

Thanks for thinking of us! We have written a
[technology report](https://www.frontiersin.org/articles/10.3389/fninf.2018.00087/full)
about this that has been published in Frontiers in Neuroinformatics.
