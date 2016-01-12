# Compilation instructions

## Requirements

  * MPI
  * OpenMP
  * Python3
  * Boost (only headers are required)
  * GCC or Intel compiler with C++11 support
    * GCC recommended for now
  * Some dependencies will be built if not found


## Compile the core

Change paths as necessary, of course

    mkdir build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=core/install/path \
          -DCMAKE_BUILD_TYPE=Debug \
          -DMPI_C_COMPILER=<MPI C compiler> \
          -DMPI_CXX_COMPILER=<MPI C++ compiler> \
          -DMPI_Fortran_COMPILER=<MPI fortran compiler> \
          ../
    make install



## Compiling modules

Modules are installed into the same top-level directory as the core.
(For now).

    cmake -DBPMODULE_PATH=core/install/path \
          -DCMAKE_INSTALL_PREFIX=core/install/path \
          -DCMAKE_BUILD_TYPE=Debug \
          ../
    make install



## Running tests

Tests can be run from the `test` subdirectory of the install directory. All paths should
be determined automatically.
