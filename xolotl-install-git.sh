#!/usr/bin/env bash

# This script should be run if you are installing via git.
# It should be run after:
#   git clone --recursive git@github.com:sg-s/xolotl.git

# It updates all submodules and sets up `git status`.
# Run it from inside the main xolotl directory.

git submodule update --init
git submodule foreach -q --recursive 'git checkout $(git config -f $toplevel/.gitmodules submodule.$name.branch || echo master)'
git config --global status.submoduleSummary true

# Set up the git hooks
# move every file from the git-hooks directory to xolotl/.git/hooks/
cd git-hooks
for f in *
do
  cp -v "$f" ../.git/hooks/"$f"
done
cd ..