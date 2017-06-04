#!/usr/bin/env python3

# This script runs an entire example input file for Pulsar.
#
# The purpose of this script is to set the appropriate PYTHONPATH for
# pulsar and then to run the example input script.
#
# Its only required argument is a path to the example script.
# Any remaining arguments are interpreted as paths to add to the search paths for
# any modules required for the example.

import os
import sys
import subprocess
import traceback

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


# Path to the examples directory
expath = os.path.join(parent, "lib", "pulsar", "examples")

# Check for the right arguments
if len(sys.argv) < 2:
  print("Usage: pulsar_runexample.py examplefile [additional python paths]")
  raise RuntimeError("Error - at least one argument required")
  quit(-1)

if len(sys.argv) > 2:
    sys.path = sys.argv[2:] + sys.path

full_path = os.path.join(expath, sys.argv[1])
if not os.path.isfile(full_path):
  raise RuntimeError("Error - path \"{}\" is not a file".format(full_path))
  quit(-2)

example_path, example_file = os.path.split(full_path)
example_name, example_ext = os.path.splitext(example_file)
pyex = sys.executable


print("---------------------------------------------------")
print(" Test information")
print("   python executable: {}".format(pyex))
print("           full_path: {}".format(full_path))
#print("        example_path: {}".format(example_path))
#print("        example_file: {}".format(example_file))
print("        example_name: {}".format(example_name))
print("         example_ext: {}".format(example_ext))
print("        python paths: {}".format(" ".join(sys.path)))
print("---------------------------------------------------")
sys.stdout.flush()

# Run the example file
my_cmd = pyex + " " + full_path

# Set up the environment
my_env = os.environ.copy()
if 'PYTHONPATH' in my_env:
  my_env['PYTHONPATH'].insert(0, ":".join(sys.path))
else:
  my_env['PYTHONPATH'] = ":".join(sys.path)

# Actually run the test, watching for exceptions
try:
  # Run the example
  ret = subprocess.Popen(my_cmd, env=my_env, shell=True).wait()
except Exception as e:
  # Printing to pulsar output here so that it doesn't get
  # interleaved with python's output
  out = pulsar.get_global_output()
  out.output("\n")
  out.output("!!! Caught unhandled exception inside an test\n")
  out.output(traceback.format_exc())
  out.output("\n")
  out.flush()
  ret = -1

# Failure is signaled via non-zero exit value
exit(ret)

