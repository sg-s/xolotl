This document describes how xolotl is tested before release, 
and our development strategy. 

## Release channels 

xolotl is developed in the open, on [Github](https://github.com/sg-s/xolotl)

* the `master` branch contains bleeding-edge releases, and should 
typically be functional. No breaking changes will be introduced in
`master` without first being built in another branch. 
* If you're comfortable with `git`, you should probably pull from
master, as that's the fastest way to get bugfixes and new features. 
* New features, or breaking changes, or architectural changes, will
be first introduced in a different branch, and will be merged 
into master when sufficiently stable. 
* We also release MATLAB toolboxes as binary releases via Github
releases. It is expected that most users (and inexperienced users)
use this channel. Releases are published only if they pass a long
string of tests on multiple operating systems. This is guaranteed to
be stable. 

## Testing toolchain 

### kapalin 

We use the [kapalin](https://github.com/sg-s/kapalin)
testing framework to run a variety of tests 
both locally and on remote machines running different operating 
systems. Currently, we test on:

* macOS
* Windows 7
* Debian
* Ubuntu

### VirtualBox

We use VirtualBox to virtualize operating systems so that 
kapalin can test the same toolbox binary on multiple operating
systems. 

### github-release-cli

`kapalin` uses `github-release-cli` to automatically
update binaries on Github if they pass all tests. 
