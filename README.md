# Compilation instructions

## Requirements

  * MPI
  * OpenMP
  * Python3
  * Boost (with python3 support)
  * GCC or Intel compiler with C++11 support
    * GCC recommended for now



## Download and compile libelemental

For now, I build static libraries, which requires the -fPIC flag. Don't
forget to change the install path.

    mkdir build && cd build
    cmake -DBUILD_KISSFFT:Bool=Off \
          -DBUILD_METIS:Bool=Off \
          -DINSTALL_PYTHON_PACKAGE:Bool=No \
          -DBUILD_SHARED_LIBS:Bool=No \
          -DCMAKE_INSTALL_PREFIX=libel/install/path \
          -DCMAKE_CXX_FLAGS="-fPIC" \
          ../



## Compile the core

Change paths as necessary, of course

    mkdir build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=core/install/path \
          -DBPMODULE_LIBEL_PATH=libel/install/path \
          -DCMAKE_BUILD_TYPE=Debug \
          ../
    make install



## Compiling modules

Modules are installed into the same top-level directory as the core.
(For now).

    cmake -DBPMODULE_PATH=/home/ben/programming/BPModule/install \
          -DCMAKE_INSTALL_PREFIX=core/install/path \
          -DBPMODULE_LIBEL_PATH=libel/install/path \
          -DCMAKE_BUILD_TYPE=Debug \
          ../
    make install



## Running tests

Scripts in `test` can be run as is, with the exception that the path
at the top has to be changed to the install path. Dirty hack, I know.
Sorry about that :(
