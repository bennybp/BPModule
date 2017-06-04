Compiling and Installing Pulsar                          {#building}
===========================================

## The Pulsar-Meta package

The Pulsar-Meta package was created to help with downloading
the correct versions of some of the requirements, since
many are unfamiliar to most users. See https://github.com/pulsar-chem/Pulsar-Meta
for details.

\note This page pertains only to building and installing the core. If you
are using the Pulsar-Meta package, see that documentation for details.

## Requirements

\todo Better compiler list. Does GCC 4.9 actually work?
\todo Better MPI requirements

  * Linux or MacOS operating system
  * C++ compiler supporting C++14
    * Intel 2016 or above
    * GCC 4.9 or above 
  * MPI
    * OpenMPI (compiled with thread-multiple support)
    * IntelMPI
  * Python 3.3+
  * CMake 3.2+ (https://cmake.org)
  * Eigen 3 (https://eigen.tuxfamily.org)
  * Berkeley DB (https://www.oracle.com/database/berkeley-db/db.html)
    * Often called 'db' or 'libdb' in Linux package managers
  * Pybind11 (https://github.com/pybind/pybind11)
  * Memwatch (https://github.com/bennybp/memwatch)
  * BPHash (https://github.com/bennybp/bphash)
  * BPPrint (https://github.com/bennybp/bpprint)


## Building Pulsar

Pulsar is build using CMake. The basic commands to build and install
are

\code{.sh}
  mkdir build
  cd build
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to \
        /path/to/Pulsar-Core
  make install
\endcode

This will attempt to autodetect the requirements and then to build and install
using the `CMAKE_INSTALL_PREFIX` as the destination. The usual directory tree
(bin, include, lib, share) will be created under this prefix if not already existing.

See @ref filesystem for more details on the directory structure of the install
directory.

## Testing

The testing suite can be run from the build directory by
running the `ctest` command. Information about the failures
can be found in the `Testing/Temporary/LastTest.log` file. Be sure to
include this file with any bug reports about test failures.

## Common Build Options

All of the build options are controlled through CMake variables. Pulsar
uses the common, standard CMake variables.

### Compiler

The (MPI) compilers can be specified with the `MPI_CXX_COMPILER`. This can
be an absolute path if desired, othewise the usual lookup will be performed.
For example, to use the Intel MPI compiler

\code{.sh}
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to \
        -DMPI_CXX_COMPILER=mpiicc \
        /path/to/Pulsar-Core
\endcode


### Build type

The build type can be specified by `CMAKE_BUILD_TYPE`. Valid values
are `Release`, `Debug`, and `RelWithDebInfo`. The default is `Release`.

\code{.sh}
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to \
        -DCMAKE_BUILD_TYPE=Debug \
        /path/to/Pulsar-Core
\endcode


### Path(s) to dependencies

The path to any dependencies should be passed through the `CMAKE_PREFIX_PATH`
variable. This path should contain the root of an install tree for the dependency/dependencies.
That is, the directory should contain `include`, `lib`, and so forth. Multiple
paths should be joined with a semicolon.


\code{.sh}
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to \
        -DCMAKE_PREFIX_PATH="/some/path;/some/path2" \
        /path/to/Pulsar-Core
\endcode

### RPATH to dependencies

`RPATH` contains paths that are hard-coded into shared object files.
These paths are searched for other shared object libraries when
required (that is, they can be thought of as hard-coded `LD_LIBRARY_PATH`).

The `RPATH` that Pulsar is going to be installed with can be changed by
specifying the `CMAKE_INSTALL_RPATH` variable on the command line. This
is useful when you are using external dependencies (for BPHash, BPPrint, etc)
and don't want to always specify the path to those libraries via `LD_LIBRARY_PATH`.

By default, the `RPATH` is \b not set by the Pulsar build. This is due to the
fact that it may be build by other meta-build packages, in which case
Pulsar may be temporarily installed with its dependencies in a staging directory.
In that case, the `RPATH` (which must be an absolute path) will be incorrect, and
would need to be manually changed upon final installation. For details about the
`RPATH`, see the \ref shared

Typically, the `RPATH` will be set to the `lib` or `lib64` directory
of a path or paths specified as the `CMAKE_PREFIX_PATH`
 
\code{.sh}
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to \
        -DCMAKE_PREFIX_PATH="/some/path;/some/path2" \
        -DCMAKE_INSTALL_RPATH="/some/path/lib;/some/path2/lib" \
        /path/to/Pulsar-Core
\endcode
 

### Special compilation flags

Special compilation flags can be specified with `CMAKE_CXX_FLAGS` variable.
Not that in this case, separate flags are separated by spaces (this is
due to CMake's handling of that variable)

\code{.sh}
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to \
        -DCMAKE_CXX_FLAGS="-mavx -Werror" \ 
        /path/to/Pulsar-Core
\endcode


