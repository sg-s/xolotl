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

## See Also 

* [GetMD5](https://www.mathworks.com/matlabcentral/fileexchange/25921-getmd5) on the Mathworks File Exchange 
* [The MD5 algorithm](https://en.wikipedia.org/wiki/MD5)