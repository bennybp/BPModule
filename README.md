# Contents
  1. [Purpose and Overview](#purpose-and-overview)
     - [What is a Module?](#what-is-a-module?)
     - [Why a Framework?](#why-a-framework?)
     - [Benefits](#benefits)
  2. [Compiling](#compiliation-instructions)
     - [Requirements](#requirements)
     - [Compiling The Core](#compile-the-core)
     - [Compiling Modules](#compiling-modules)
     - [Running Tests](#running-tests)

# Purpose and Overview

The "Pulsar" project, as it currently is being called, is intended to be a
re-envisioned framework in which to run and develop quantum chemical computations
and methods via interchangeable modules.  Together, these two points allow one
to rapidly develop production-quality electronic structure codes, in a fraction
of the time.

## What is a Module?

In computer science a module is a pretty generic term being loosely defined as
a unit of code that: carries out a specific task, standardizes the input/output
of that task, and may enlist the help of other modules to accomplish that task.
Presently we're using a more loose definition that amounts to a task that can
be performed in several ways.  Essentially, a module is your sandbox in which you
implement one way to perform the task.  Once the code is within your module you
are in control until you return the result in the appropriate format.  This
means you can jump right to the guts of designing your new code without worrying
about making the inputs to that module or processing the outputs.

## Why a Framework?

Let's start with what a framework is.  According to the most credible source on
Earth (Wikipedia), a framework is an "...abstraction in which software providing
generic functionality can be selectively changed by additional user-written code,
 thus providing application-specific software."  So what does that mean?
Basically, we put together a lot of infrastructure that is capable of running an
electronic structure code, but has blank spaces at all the key points.  At these
key points (say computing an energy) we defer control to you and your module
takes over.  An analogy would be a DVD and a DVD player.  The framework is the
DVD player and the DVD is your module.  The DVD player knows how to interpret
the data on the DVD and display then display it, but it needs
you to put the data on the DVD.


## Benefits

So why do you want to use our framework at all?  Admittedly at the superficial
level presented here, it may seem like you get a similar deal
developing for your package of choice.  After all, most packages have some
standard way of obtaining a basis set or a molecule.  However, what we really
are purposing here goes beyond modules for simple data and is meant to
eventually permeate down to the very bowels of an electronic structure code.
Imagine switching integrals out at runtime or writing a Hartree-Fock code that
works with any integral library!!! Depending on what code(s) you have written or
worked with, this may seem farfetched, but that sort of interchangeability is
actually pretty standard in computer science.

We see the main reasons to use our framework as:

- Easy production-level prototyping of new methods
  - Modules can call other modules
  - Other modules are production-level already
- Interoperability
  - Modules associated with one package can be used with another
  - Our interface is designed to be non-invasive, i.e. your code is likely fine
    as is and simply requires an adaptor class to function
- Support for coarse and fine-grained parallelism
  - Tools for writing task-based and data-based parallelism that scales to 
    thousands of cores, in an easy and intuitive manner
- Standards/ Best Practices/ Developer's Manual
  - We strive to make our code practices transparent and readily available

# Compilation Instructions

Great.  We convinced you to at least try our framework.  So how do you go about
doing that?  Well we tried our best to make this as painless as possible as long
as you follow the directions below.  If the directions are not clear or you find
a problem, please let us know so we can make this even more painless.

## Requirements
  
You must have the following already compiled and available:
  * MPI version 2.0 or higher
    * Used for distributed parallelism of the project
  * OpenMP 
    * Used for some of the shared parallelism
  * Python3
    * Used as one interface language
    * Used to generate source files
  * CMake version 3.0 or higher
  * GCC or Intel compiler with C++11 support
    * GCC recommended for now, but Intel is regularly tested too
  * Basic Linear Algebra System (BLAS) library
    * Needed for math

The following are our current dependencies (and their dependencies).  If we can
not locate them on your system they will be built for you using the settings
provided to the main project.
  * PyBind11 [Link on GitHub] (https://github.com/wjakob/pybind11.git)
    * Light-weight, higher-capability replacement for boost::python
    * Library is header only except for the tests
  * LibTaskForce [Link on GitHub] (https://github.com/ryanmrichard/LibTaskForce.git)
    * Library designed to offer task-based parallelism using both shared an distributed computing paradigms
    * Depends on Madness [Link on GitHub] (https://github.com/m-a-d-n-e-s-s/madness.git)
  * Tensor [Link on GitHub] (https://github.com/ryanmrichard/Tensor.git)
    * Data-based pair for LibTaskForce aimed at hybrid parallelism of tensor equations
    * Tiled Array [Link on GitHub] (https://github.com/ValeevGroup/tiledarray.git)
      * Used to implement the guts of Tensor
      * Also depends on Madness
      * Depends on Eigen [Link on Bitbucket] (https://bitbucket.org/eigen/eigen/) 

## Compile the core

The Pulsar project is comprised of two components: the core framework and the
 modules.
The core is the framework that keeps all the modules playing nicely with one
another and we will compile that first.  You should only have to compile the
framework once unless you are developing for it. Note that Pulsar uses git
submodules, which are a bit touchy sometimes.  Make sure you clone the
source code with:
```git
git clone --recursive git@github.com:pulsar-chem/Pulsar-Core.git
```
If you forget to do that in the Pulsar home directory run:
```git
git submodule update --init --recursive
```
which will then checkout all of the submodules for you.


Once you have the source code (and the submodules), compilation of the core 
should be as simple as (on an ideally setup Linux box):

```Bash
#In the Pulsar root directory
cmake -H. -Bbuild
cd build && make
make install
```

If your compilers, libraries, etc. are not in common locations you may want/need
 to set some or all of the following CMake variables:

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
PULSAR_MATH_LIBS=<-mkl or other linking flags>

# These are more fine-granined control
CMAKE_PREFIX_PATH=<Paths that CMake should search when looking for dependencies>
CMAKE_BUILD_TYPE=<Debug or Release>
CMAKE_INSTALL_PREFIX=<Path where the Pulsar gets installed>
```

## Compiling modules

Modules are installed into the same top-level directory as the core.
(For now).

    cmake -DPULSAR_PATH=core/install/path \
          -DCMAKE_INSTALL_PREFIX=core/install/path \
          -DCMAKE_BUILD_TYPE=Debug \
          ../
    make install



## Running tests

Tests can be run from the `test` subdirectory of the install directory. All paths should
be determined automatically.
