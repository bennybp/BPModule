Obtaining, Compiling, and Installing Pulsar                          {#building}
===========================================

Build types and C++ standards
-------------------------------

By default, Pulsar is build as a `Release` build. This can be changed
by specifying `CMAKE_BUILD_TYPE` on the command line.

Pulsar is also built with C++14. This can be changed via `CMAKE_CXX_STANDARD`
and `CMAKE_CXX_EXTENSIONS`, although at least C++14 is required.
See the CMake documentation for details.


Specifying custom paths
-------------------------------

Most of the time the autodetection of the dependencies from CMake will
work well enough. However, if it doesn't find the correct dependencies,
the paths to the various dependencies can be fine-tuned via
the following arguments to CMake

- `CMAKE_PREFIX_PATH` - Additional paths to search for dependencies

- `PYTHON_EXECUTABLE` - Which python executable to use (also used to find
                        python libraries)

- `MPI_CXX_COMPILER` - Which MPI wrapper to use (ie, `mpiicpc`)

- `EIGEN3_INCLUDE_DIR` - Path to Eigen3 headers (directory
                         contains the `Eigen` subdirectory)

- `pybind11_DIR` - Path to custom pybind11 install (directory contains the
                   `pybind11Config.cmake` file)

- `libtaskforce_DIR` - Path to custom LibTaskForce install (directory contains the
                       `libtaskforceConfig.cmake` file)

- `bphash_DIR` - Path to custom BPHash install (directory contains the
                 `bphashConfig.cmake` file)

- `bpprint_DIR` - Path to custom BPPrint install (directory contains the
                 `bpprintConfig.cmake` file)

- `memwatch_DIR` - Path to custom Memwatch install (directory contains the
                   `memwatchConfig.cmake` file)

TODO: berkeleyDB
