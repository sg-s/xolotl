This document describes how xolotl and cpplab use
hashing to verify the  identity of models and to select
the correct binary to use.

## What is a hash?


A hash is a short string that is used to uniquely identify a
arbitrary-length piece of data. A hash is created by a hash function
that accepts data of any length, and returns a string of a fixed
length. It may seem impossible that any data of any length can
be uniquely identified by a short string; and this is true. In
practice, however, a good hash function avoids collisions of these
sorts, and most data resolves into unique hashes.


## Why hash?

xolotl uses hashes to identify models. Since models can be nested
and hierarchical and sometimes quite large and complex, a hash
is a convenient way or referring to a model.

## How models are hashed

The structure of the xolotl object is what is hashed. This means that the combination
of network components (e.g. compartments, conductances, etc.) with their names and connections
is assigned a cryptographic string of alphanumeric characters. We use the well-studied MD5
algorithm. Crucially, parameters of the model are _not_ hashed. This means that
_families of models_, those with the same structure/equations but different parameter values
have the same hash.

When a change is performed to the xolotl object in MATLAB, a new compiled C++ binary
is created _only_ if the hash changes (i.e. the structure of the model changed). This means
that running models that only differ by parameters is extremely fast and easy.

When the [`integrate`](../../reference/matlab/xolotl/#integrate) method is called, the hash of the xolotl object is compared
with the hash of the compiled binaries and the correct one is chosen. The xolotl
object is [`serialize`](../../reference/matlab/cpplab/#serialize)d to get a list of parameters, which are passed as function arguments.
This way, you only need one compiled binary per unique xolotl structure. Parameters can
change freely.

## See Also

* [GetMD5](https://www.mathworks.com/matlabcentral/fileexchange/25921-getmd5) on the Mathworks File Exchange
* [The MD5 algorithm](https://en.wikipedia.org/wiki/MD5)
