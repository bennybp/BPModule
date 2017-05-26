#!/usr/bin/env python3

# This script runs a test for Pulsar.
#
# The purpose of this script is to import pulsar from the appropriate
# path (relative to this file) and also do some common initialization
# of the core. It also handles cleanup of the core as well.
#
# Its only required argument is a path to a script that actually does the testing.
# Any remaining arguments are interpreted as paths to add to the search paths for
# any modules required for the test.
#
# The return value is zero if everything runs ok and the test script
# returns zero. Otherwise, if there is a problem with this script
# (bad arguments, etc), or if an unhandled exception escapes the test script,
# the return value is negative. If there are failing tests, the return value
# of the test script (and therefore this script) should be greater than zero.

import os
import sys
import traceback
from importlib.machinery import SourceFileLoader, ExtensionFileLoader

# Import pulsar - slightly tricky
# This file resides in <install_path>/bin
# So we add the corresponding lib path to the python path
# so we can import the pulsar core package
thispath = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(thispath)

# Find the pulsar library directory
modpath = os.path.join(parent, "lib", "pulsar", "modules")
sys.path.insert(0, modpath)

import pulsar


# Check for the right arguments
if len(sys.argv) < 2:
  print("Usage: pulsar_runtest.py testfile [additional python paths]")
  raise RuntimeError("Error - at least one argument required")
  quit(-1)

if len(sys.argv) > 2:
    sys.path = sys.argv[2:] + sys.path

full_path = sys.argv[1]
if not os.path.isfile(full_path):
  raise RuntimeError("Error - path \"{}\" is not a file".format(full_path))
  quit(-2)

test_path, test_file = os.path.split(full_path)
test_name, test_ext = os.path.splitext(test_file)

print("---------------------------------------------------")
print(" Test information")
print("     full_path: {}".format(full_path))
#print("     test_path: {}".format(test_path))
#print("     test_file: {}".format(test_file))
print("     test_name: {}".format(test_name))
print("      test_ext: {}".format(test_ext))
print("  python paths: {}".format(" ".join(sys.path)))
print("---------------------------------------------------")
sys.stdout.flush()

# Initialize pulsar
# From now on, output should go through pulsar
pulsar.initialize(sys.argv,
                  color = False,
                  debug = True,
                  outbase = "psr_output_" + test_name)

# We can't do the normal import. Some tests may have the
# same name, which leads to a test_name.py and test_name.so
# So we have to do it manually, given the full path

# Slightly different, depending on the test type
if test_ext == ".py":
  m = SourceFileLoader(test_name, full_path).load_module()
else:
  m = ExtensionFileLoader(test_name, full_path).load_module()


# Actually run the test, watching for exceptions
try:
  # Run the test in the imported (python) module
  ret = m.run_test()
except Exception as e:
  # Printing to pulsar output here so that it doesn't get
  # interleaved with python's output
  out = pulsar.get_global_output()
  out.output("\n")
  out.output("!!! Caught unhandled exception inside a test\n")
  out.output(traceback.format_exc())
  out.output("\n")
  out.flush()
  ret = -1

# Failure is signaled via non-zero exit value
exit(ret)

