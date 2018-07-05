.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _compilers:
Compiler Support
================

Compilers for which ``xolotl`` has been known to work (or not).

=================== ============ ============================ ======
 **OS**             **Compiler**     **Version**              **Works**
 macOS 10.12.6        clang        Apple LLVM version 9.0.0    ✓
 Ubuntu 18.04 LTS     g++          7.3                         ✓
 Ubuntu 16.04.3 LTS   g++          6.3                         ✓
 Ubuntu 17.04         g++          4.8.5                       ✓
 Ubuntu 17.04         g++          6.3                         ❌
 Ubuntu 17.04         g++          7.0.1                       ❌
 Ubuntu 17.10         g++          6.4                         ❌
 Ubuntu 17.10         g++          7.3                         ❌
 Ubuntu 17.10         g++          4.8                         ✓
 Manjaro Linux        g++          7.3                         ✓
=================== ============ ============================ ======

Compiling on Linux
^^^^^^^^^^^^^^^^^^

There are a couple of specific quirks to using the ``MEX`` compiler on Linux machines.
First, make sure that you have a ``C++`` compiler installed. We recommend ``g++``, which
is part of the ``GCC`` `software project`__. On Debian/Ubuntu systems, look for something similar to ::

  sudo apt-get install g++

On Arch-based systems such as Manjaro, do something similar to ::

  sudo pacman -Syu gcc6

.. hint::

  ``MATLAB`` can be particular about which version of ``g++`` it works with. For best results, use
  the compiler version recommended by ``MATLAB``. In addition, it's best to point ``MATLAB`` towards
  the system compiler, rather than one installed through distributions like ``Anaconda``.

__ https://gcc.gnu.org/

On certain Linux distributions (Arch-based in particular), ``MEX`` cannot identify
the installed ``g++`` compiler, even when it exists on your path. The error
looks something like this: ::

  >> !which g++
  /bin/g++
  >> !g++ -dumpversion
  8.1.0

``MATLAB`` cannot find the intended ``C++`` compiler. ::

  >> mex -setup C++
  Error using mex
  No supported compiler was found.

``MATLAB`` recommends changing your PATH so that you default to an older version
of ``g++``. This is not strictly necessary: ``MATLAB`` can still compile using ``MEX``
with newer versions of ``g++`` in most cases. Generally, downgrading to an older
version of ``g++`` doesn't solve the problem.

There is a relatively simple fix however. Credit goes to ``github`` user bonanza123_
for figuring it out.

.. _bonanza123: https://gist.github.com/bonanza123/
.. _here: https://gcc.gnu.org/


* First download the proper version of ``gcc/g++``. If you use a package manager there are generally legacy versions under ``gcc-VERSION``. You can also find them here_.
* Second change the ``mex_LANG_glnxa64.xml`` specification file, where ``LANG`` is either ``C`` or ``C++``. This is typically found at ``~/.matlab/R2018a/mex_C_glnxa64.xml`` where ``R2018a`` is the version of ``MATLAB`` and ``C`` is the name of the language.
* Replace all references to ``$GCC`` with the path to the soft link to your ``gcc`` compiler (e.g. ``/usr/bin/gcc-6``).
* Repeat this process for the ``./mex_C++_glnxa64.xml`` file.
* Sometimes ``MATLAB`` doesn't generate the ``C++`` ``.xml`` file, causing a lot of errors. If it doesn't exist, copy the ``C`` verson of the file, rename it appropriately, and replace all references to ``gcc`` with ``g++``, so that ``MATLAB`` knows to use the right compiler.


The problem is fixed if you see something like this in ``MATLAB`` ::

  >> mex.getCompilerConfigurations('C++')
  ans =

  CompilerConfiguration with properties:

             Name: 'g++'
     Manufacturer: 'GNU'
         Language: 'C++'
          Version: ''
         Location: '/usr/bin/g++-6'
        ShortName: 'g++'
         Priority: 'A'
          Details: [1×1 mex.CompilerConfigurationDetails]
       LinkerName: ''
    LinkerVersion: ''
           MexOpt: '/home/alec/.matlab/R2018a/mex_C++_glnxa64.xml'
