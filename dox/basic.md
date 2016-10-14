Basic File System Layout                                                {#basic}                                 
============

We divide the Pulsar filesystem into two areas: source and install.  As you can
guess, source is where the source files for Pulsar live and install is where
the finished project lives.

## Source files

After obtaining the Pulsar source from GitHub all files will live inside a
directory called Pulsar-Core (unless you decided to name it something else).
This is what we call the root directory.  At the moment the folders inside are:

- basis: The parameters for Gaussian basis sets (should probably not be
  displayed so prominently)
- dox: The source files for the documentation
- external: Contains source files for libraries needed for Pulsar that the user
  is not likely to already have
- pulsar: The main source directory for the Pulsar framework
- scripts: Scripts for automatically making various source files
- tests: Tests contain unit and acceptence tests see [testing](@ref testing)

If you are going to contribute to the core you are most likely going to be
working on source files conatined in the pulsar directory.  This directory is
laid out as:

- datastore: source files related to the cache and options.  If it has to do
  with generically handeling data it probably belongs here.
- exception: source files for the various types of errors contained in Pulsar
- math: Pulsar core uses Eigen for its matrix needs; however, there are still a
  few additional math needs such as combinitorial coefficients, set theory, and
  number theory that are needed.  The source for that lives here as does the
  source for wrapping generic tensor classes.
- modulebase: this is where the source files for the various types of modules
  live.  These are the authoritative definitions of the module APIs.  If it is
  a new module type the source goes here.
- modulemanager: Source files related to handeling module creation/deletion as
  well as synchornization go here.  This is the low level guts of Pulsar.
- output: This is source files for functions related to printing.  All printing
  should go through these functions/objects
- parallel: A superficial interface to LibTaskForce probably should be merged
  with modulemanager as this is what provides the parallel resources to modules
- python: Source files for the C/python interface of the modulemanager
- system: Source files related to the molecule, its environment, and the basis
  set
- testing: Some functions for testing various classes as well as handeling basic
  common testing patterns
- util: A bit of a hodge podge of useful computer sciencey things that don't fit
  in well above.  Things like memory watching and hashing live here

## Install directory

When building Pulsar for the first time you will have to choose a place to
install it (controlled by CMAKE_INSTALL_PREFIX defaults to /usr/local on Linux
and Mac).  Whatever value you set for this value is what we will call the
install root.  If you are familiar with Latex's installation this is somewhat
similar.  Basically there are a set of directories in Pulsar's install root
that are the default locations it checks at runtime for content.  As you expand
your set of Pulsar modules you install them here (just like putting .sty files
in texmf/tex/latex).  You do not have to put them there, but if you don't then
you will have to tell Pulsar where to look.  Particularly for developing it may
be desirable to have two directories with the install structure, the real one,
which is where the known working modules are installed, and a buffer one, which
is where the module you are currently working on lives.

The install root structure is:

- basis: this is where Pulsar looks for basis sets.  Initially this is only
  populated by the basis sets that come with the pulsar core
- external: this is the install location of external dependencies of Pulsar that
  the Pulsar build system built.  Modules are encouraged to also use these
  libraries, which is why they are here.
- include: the header files for the pulsar C-interface.  If you are designing a
  new module in C this is where you get the Pulsar API from
- modules: this is the directory where all modules are installed.  Each set of
  modules should be namespace protected by the supermodule name.  By default
  you only have the pulsar core modules, which are modules in the Python sense
  and not the Pulsar sense.  All other modules in this directory should be
  actual Pulsar modules

