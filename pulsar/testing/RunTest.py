import os
import sys
import traceback
from importlib.machinery import SourceFileLoader, ExtensionFileLoader

# Import pulsar - slightly tricky
# This file resides in <install_path>/modules/pulsar/testing.
# So we add our grandparent to the python path so we can import pulsar
thispath = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(thispath)
gparent = os.path.dirname(parent)
sys.path.insert(0, gparent)

import pulsar

# Manually import the test file via the full path
# Add the file's directory to the python path
full_path = sys.argv[1]
test_path, test_file = os.path.split(full_path)
test_name, test_ext = os.path.splitext(test_file)

print("---------------------------------------------------")
print(" Test information")
print("  full_path: {}".format(full_path))
print("  test_path: {}".format(test_path))
print("  test_file: {}".format(test_file))
print("  test_name: {}".format(test_name))
print("   test_ext: {}".format(test_ext))
print("---------------------------------------------------")
sys.stdout.flush()

# We can't do the normal import. Some tests may have the
# same name, which leads to a test_name.py and test_name.so
# So we have to do it manually, given the full path

# Slightly different, depending on the test type
if test_ext == ".py":
  m = SourceFileLoader(test_name, full_path).load_module()
else:
  m = ExtensionFileLoader(test_name, full_path).load_module()


# set up pulsar
pulsar.initialize(sys.argv,
                  color = False,
                  debug = True,
                  outbase = "psr_output_" + test_name)

# Actually run the test, watching for exceptions
try:
  # Run the test in the module
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

# Done with pulsar now
pulsar.finalize()

# Failure is signaled via non-zero exit value
exit(ret)
