.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _faq:
Frequency Asked Questions (FAQ)
===============================

Who Wrote xolotl?
^^^^^^^^^^^^^^^^^
``xolotl`` was designed and written by `Srinivas Gorur-Shandilya`_ and Alec Hoyland in
Eve Marder's laboratory_ at Brandeis University.

.. _`Srinivas Gorur-Shandilya`: https://srinivas.gs/
.. _laboratory: https://blogs.brandeis.edu/marderlab/

How Can I Contribute?
^^^^^^^^^^^^^^^^^^^^^
``xolotl`` is far from complete and contributions are welcome. Check out our guide_
on how to report bugs, add conductances and mechanisms, or contribute to the code base.

.. _guide: contributing.rst

Something is Broken, What Do I Do?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
As the Hitchiker's Guide to the Galaxy reads, "DON'T PANIC". The most common error
we see is an issue between ``MATLAB``, the ``MEX`` compiler, and your computer. In that
situation, a first step would be to check our compilers_ guide which has detailed fixes for
some of the most common compiler issues we've seen.

.. hint::
  If ``GetMD5`` fails, then it's definitely at least a compiler issue.

If it's not a compiler issue and you've checked all the typical programming
pitfalls (syntax, spelling, your ``MATLAB`` path, cleared your workspace, etc...),
try the following ::
  % erases all the compiled binaries of xolotl networks
  x.cleanup

If you're still having a problem, we might have fixed it already! Reinstall or update
by running the `install script` or pulling from the repository if you're using ``git``.
Remember that if you're using ``git`` you should be sure to pull all the dependencies as well
(``srinivas.gs_mtools``, ``cpplab``, ``puppeteer``).

.. _compilers: compilers.rst
.. _`install script`: installing.rst

How Do I Cite This?
^^^^^^^^^^^^^^^^^^^
Thanks for thinking of us! We'll be publishing later this year, and putting a
manuscript on ArXiv_ sooner than that. For now though,

*Gorur-Shandilya, Hoyland, & Marder, in preparation* should be sufficient.

.. _ArXiv: https://www.biorxiv.org/content/early/2018/08/18/394973

How Do I Install It?
^^^^^^^^^^^^^^^^^^^^
``xolotl`` can be installed into your home folder by a simple script_. For advanced
users, feel free to clone from our github repository_.

.. _script: installing.rst
.. _repository: https://github.com/sg-s/xolotl
