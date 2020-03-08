This document describes how we generate documentation for 
xolotl, and why we made the decisions we did.

## Philosophy 

### Self-documenting code 

Ideally, one should be able to read the code and understand 
what it does. However, it's often not easy to understand
*why* it does what it does, or what the bigger picture is. 

In any case, we believe that documentation should be tightly 
coupled to the code it refers to, where possible. For example,
it makes sense to include the documentation on how to use a 
certain method in the code for the method itself. This is what we 
have done for every single method of xolotl. If you open a file
describing a xolotl method, you will find the documentation for it
in that file. 

### Documentation should be built, not written 

If we have documentation for each method mixed up with the code, 
then how do we put them all together in one place in the 
documentation? Obviously the answer is to programatically build 
the documentation from source (code), so that 

* we don't deal with copies of the same documentation in different places that we need to keep in sync
* changes in the source code automatically lead to changes in the documentation 

By "built", we mean that we use tools to automatically generated 
markdown files or HTML files from source code. 

### Simpler is better

Markdown has emerged as the near-universal standard in simple,
yet powerful markup on the web. The nice thing about markdown
is that it's perfectly readable as plain text too. This means
that if something goes catastrophically wrong with your 
documentation build system, all is not lost, as people can 
still read the markdown source. 

Therefore we write our documentation in markdown, either in 
standalone files, or inline with code, and generate nice
HTML files using mkdocs.

!!! info "A note to the pythonistas"
    We tried RestructuredText, and sphinx, and it was **awful**. Don't believe anyone who says it's good. 

## Toolchain 

### MKDocs

We like using mkdocs because:

* it supports markdown
* it's very easy to setup, and use
* it can also serve docs locally, for testing

### Material theme for MKdocs

This is a very nice-looking theme that I like. 
You're looking at it now. [Link to project page](https://squidfunk.github.io/mkdocs-material/)

## Hosting

Currently, our documentation is hosted on readthedocs.io. This
may change, but this URL:


[https://go.brandeis.edu/xolotl](https://go.brandeis.edu/xolotl)

will always point to the documentation