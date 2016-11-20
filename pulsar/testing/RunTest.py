import os
import sys
import importlib

# Import pulsar - slightly tricky
# This file resides in <install_path>/modules/pulsar/testing.
# So we add our grandparent to the python path so we can import pulsar
thispath = os.path.dirname(os.path.realpath(__file__))
moddir = os.path.realpath(os.path.join(thispath, "../../"))
sys.path.insert(0, moddir)

import pulsar

# Manually import the test file via the full path
# Add the file's directory to the python path
test_path, test_name = os.path.split(sys.argv[1])
sys.path.insert(0, test_path)
m = importlib.import_module(test_name)

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

