import os
import sys
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

print(" Test information")
print("  full_path: {}".format(full_path))
print("  test_path: {}".format(test_path))
print("  test_file: {}".format(test_file))
print("  test_name: {}".format(test_name))
print("   test_ext: {}".format(test_ext))

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
                  outbase = "psr_test_output")


# Run the test in the module
nfailed = m.run_test()

# Done with pulsar now
pulsar.finalize()

# Failure is signaled via non-zero exit value
exit(nfailed)

