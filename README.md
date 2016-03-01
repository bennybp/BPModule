# Contents
  1. [Purpose](#purpose)
  2. [Compiling](#compiling-instructions)
     - [Requirements](#requirements)
     - [Compiling The Core](#compile-the-core)
     - [Compiling Modules](#compiling-modules)

# Purpose

The "BPModule" project, as it currently is being called, is intended to be a
re-envisioned framework in which to run and develop quantum chemical computations
and methods.  The key idea is modularity.

# Compilation instructions

## Requirements
  
You must have the following already compiled and available:
  * MPI
  * OpenMP
  * Python3
  * CMake version 3.0 or higher
  * Boost (only headers are required)
  * GCC or Intel compiler with C++11 support
    * GCC recommended for now

The following are our current dependencies (and their dependencies).  In all cases
these will be built if we do not find them.
    * PyBind11 [Link on GitHub] (https://github.com/wjakob/pybind11.git)
    * LibTaskForce [Link on GitHub] (https://github.com/ryanmrichard/LibTaskForce.git)
        * Madness [Link on GitHub] (https://github.com/m-a-d-n-e-s-s/madness.git)
    * Tensor [Link on GitHub] (https://github.com/ryanmrichard/Tensor.git)
        * Tiled Array [Link on GitHub] (https://github.com/ValeevGroup/tiledarray.git)
            * Also depends on Madness
            * Eigen [Link on Bitbucket] (https://bitbucket.org/eigen/eigen/) 
              * At the moment we have to "build" this for you (Eigen is a 
                header-only library so there's not really any building)

## Compile the core

The BPModule project is comprised of two components: the core and the modules.
The core is the framework that keeps all the modules playing nicely with one
another and we will compile that first.  Compilation of the core should be as
simple as:

```Bash
#In the BPModule root directory
cmake -H. -Bbuild
cd build && make
make install
```

If your compilers, libraries, etc. are not in common locations you may want to
set some or all of the following CMake variables:

```Bash

#The below tune what I'll call Compiler/System dependencies:

CMAKE_C_COMPILER=<Path to C compiler>
MPI_C_COMPILER=<Path to the MPI wrapper around CMAKE_C_COMPILER>
CMAKE_CXX_COMPILER=<Path to C++ compiler>
MPI_CXX_COMPILER=<Path to the MPI wrapper around CMAKE_CXX_COMPILER>
#Note the capitalization on the next two
CMAKE_Fortran_COMPILER=<Path to Fortran compiler>
MPI_Fortran_COMPILER=<Path to the MPI wrapper around CMAKE_Fortran_COMPILER>
PYTHON_LIBRARY=<path to the Python libraries>
PYTHON_INCLUDE_DIR=<path to Python's header files>
PYTHON_EXECUTABLE=<path to the Python interpreter associated with the above>
BPMODULE_MATH_LIBS=<-mkl or other linking flags>

# These are more fine-granined control
CMAKE_PREFIX_PATH=<List of paths that CMake should search when looking for dependencies>

CMAKE_BUILD_TYPE=<Debug or Release>
CMAKE_INSTALL_PREFIX=<Path where the BPModule gets installed>
```




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
