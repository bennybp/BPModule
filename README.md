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
In our opinion Pulsar represents quite the departure from conventional chemistry
paradigms regarding program design.  The next several paragraphs and sections
attempt to succiently summarize the basics of Pulsar and hopefully provide you a
tantalizing glimpse into the future of computational chemsitry.

There is a push in computational chemistry to increase interoperability among
programs.  To this extent many groups are now designing code in a library-like
fashion so that it can be included in many packages.  With
current funding initiatives this is likely to continue and to become the norm.  
Pulsar has grown by anticipating this paradigm shift and aims to provide a
framework for aiding in workflows from low-level routines such as integrals
to high-throughput screening of ligand-protein interactions.  Ultimately, Pulsar
aims to usher in an "App Store" like era of computational cheistry where users
and developers modify the capabilities and functionality of Pulsar by installing
additional "apps".  Like apps on a phone we encourage apps to call one another
to perform tasks (similar to how your browser opens your default email app when
you click an email address, or your default directions app when you click a 
street address) rather than rewriting that functionality.  For example, when you
want to write an MP2 module you call the default SCF module to get a reference
determinant or have your high-throughput screening module call the default
scoring function.

## Modules and Frameworks

In the app store analogy, modules are the apps and the actual Pulsar source code
is your phone's operating system.  The apps are by far the cooler part and that's
what Pulsar lets you focus on.

In Pulsar lingo, what we were calling "apps" above is what we call a module.  A
module is an independent library written by a group.  Ideally
it performs one task, say optimizes a geometry, computes the SCF energy,
computes a block of integrals, etc.  Each module of a certain type, say an SCF
module, has exactly the same interface (the outer most function call to your
library), which allows them to be ``plug and play" in the sense that you can
pull one SCF module out and drop another SCF module in without affecting the
rest of the code (you can do this at runtime which as we explain below allows
some pretty cool stuff).  Modules are how you customize and extend Pulsar's
capabilities.  Want to run a density-fit SCF?  Drop in an SCF module that uses
density-fitting.  Want to run an SCF that uses some new trick no one else has
ever done?  Code it up and drop it in, all correlated methods will automatically
be able to use it. 

The actual Pulsar code forms what in computer science is called a framework.  It
is the scaffolding on which you build a program.  In the present case it is the
guts of a typical computational chemistry program, things like basis set parsing,
and option parsing.  Pulsar is not by itself a computational chemistry program
instead we envision future programs will be something like "My awesome program"
powered by Pulsar.  Where your awesome program provides the details of how SCF,
MP2, etc. run.

## Benefits

We see the main reasons to use Pulsar as:

- Easy production-level prototyping of new methods
  - Modules can call other modules
  - Other modules are production-level already
- Interoperability
  - Modules associated with one package can be used with another
  - Our interface is designed to be non-invasive, i.e. your code is likely fine
    as is and simply requires a simple function wrapper
- Support for coarse and fine-grained parallelism
  - Tools for writing task-based and data-based parallelism that scales to 
    thousands of cores, in an easy and intuitive manner
  - Ability to "hide" parallelism details from your module
    - If you don't want to worry about thread safety, then don't. Pulsar can make
      sure your lack of thread safety doesn't affect other modules
- Automatic checkpointing and restart capabilities
  - Pulsar is designed from the ground up to be able to handle these tasks more
    or less for you.  All you have to do is tell Pulsar what data is important
    and we'll save it for you in the event of crash
- Standards/ Best Practices/ Developer's Manual
  - We strive to make our code practices transparent and readily available

# Compilation Instructions

Great.  We convinced you to at least try Pulsar.  So how do you go about
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
  * Eigen matrix library
  * BerkeleyDB (it's on almost every Linux box by default).

The following are our current dependencies (and their dependencies).  If we can
not locate them on your system they will be built for you using the settings
provided to the main project.
  * PyBind11 [Link on GitHub] (https://github.com/wjakob/pybind11.git)
    * Light-weight, higher-capability replacement for boost::python
    * Library is header only except for the tests
  * LibTaskForce [Link on GitHub] (https://github.com/ryanmrichard/LibTaskForce.git)
    * Library designed to offer task-based parallelism using both shared an distributed computing paradigms
    * Depends on Intel Threading Building Blocks


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
