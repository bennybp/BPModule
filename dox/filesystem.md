# Filesystem Layout              {#filesystem}

We divide the Pulsar filesystem into two areas: source and install.  As you can
guess, source is where the source files for Pulsar live and install is where
the finished project lives.

## Install directory

The top-level install directory contains the typical linux filesystem layout
(bin, include, lib, and share). All of these directories will contain
the `pulsar` subdirectory, under which Pulsar is actually installed.
Note that more than Pulsar may be contained in this root directory if
installing from the Pulsar-Meta package.

- \b bin Helper scripts
- \b include Headers for pulsar (contained entirely under the `pulsar` subdirectory).
- \b share Common information for Pulsar. This primarily contains the
  Pulsar CMake configuration files (for use when building modules).
- \b lib The core of Pulsar itself (contained under the `pulsar` subdirectory)
  - \b basis Common basis sets in computational chemistry
  - \b examples Examples (for the user and for documentation)
  - \b modules Actual modules used by Pulsar (including the core). Each
       directory represents a supermodule, with the directory being the name.
  - \b systems Common systems (useful for testing)

In `lib/pulsar/modules/pulsar` is the actual code for the pulsar core. This
is the top of a python package and the heart of the framework.

## Source files

After obtaining the Pulsar source from GitHub all files will live inside a
directory called Pulsar-Core (unless you decided to name it something else).
This is what we call the root directory.  At the moment the folders inside are:

- \b lib Some common data/library files
  - \b basis Basis sets for calculations
  - \b examples Examples (for the user and for documentation)
  - \b systems Various systems useful for tests
- \b dox The source files for the documentation
- \b pulsar The main source directory for the Pulsar framework
- \b scripts Scripts for automatically making various source files
- \b tests Tests contain unit and acceptence tests (see @ref testing)

The main layout of of the \b pulsar subdirectory is as follows

- \b bin Some scripts that may be used by the end user
- \b datastore Various data storage classes. This includes the cache, options,
  and the Wavefunction class.
- \b exception The main exception class (pulsar::PulsarException)
- \b math Various mathematical functions and classes. This includes vector/matrix/tensor
  base classes, and pulsar::MathSet, and many other functions.
- \b modulebase Abstract base classes for the different types of modules.
  These are the authoritative definitions of the module APIs.
- \b modulemanager Functionality related to handling module loading and creation.
  Some caching and graph generation code is also here.
- \b output Functions and classes related to printing. All printing
  should go through these functions/objects, as that will correctly handle
  threading and distributed printing.
- \b parallel Very basic threading and MPI capability (mostly related to initializing
  and finalizing MPI).
- \b python Helper functions for the C++/python interfaces
- \b system: Functionality related to the molecule, its environment, and the basis
  set
- \b testing Some functions for testing various classes as well as handling basic
  common testing patterns
- \b util Miscellaneious helper functions (such as hashing and memory management)

