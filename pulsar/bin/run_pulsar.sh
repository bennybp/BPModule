#!/bin/bash

# This wraps an arbitrary command in order to correctly set
# up the LD_LIBRARY_PATH for some dependencies (namely, memwatch).

# In a final, installed pulsar, this may be redundant (depending on
# if the user fixes LD_LIBRARY_PATH themselves). However,
# we want to run some tests from within the build directory, so
# we must find memwatch, etc, without playing with RPATH, which
# is a pain with CMake superbuilds

# Path to this script
MY_PATH=$(dirname "$0")
MY_PATH=$(realpath "${MY_PATH}")

# This file is installed in bin, so
# we need to go up a directory
PULSAR_PATH=$(dirname "${MY_PATH}")

# The lib path
PULSAR_LIBPATH="${PULSAR_PATH}/lib"

# Prepend that directory to the LD_LIBRARY_PATH
export LD_LIBRARY_PATH="${PULSAR_LIBPATH}:${LD_LIBRARY_PATH}"

# Preload memwatch?
export LD_PRELOAD="${PULSAR_LIBPATH}/libmemwatch.so"

# Execute the rest of the command
$@
