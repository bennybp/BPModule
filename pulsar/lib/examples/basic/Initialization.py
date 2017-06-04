import sys

# pulsar should be in your PYTHONPATH
# This path should point to the 'modules' subdirectory
# (ie PYTHONPATH=<install_prefix>/lib/pulsar/modules)
import pulsar

# Initializes the pulsar framework
# sys.argv = passing in the command line
# debug = Enable global debugging
# color = Colorize the output
# outbase = Base of the filename for output
pulsar.initialize(sys.argv,
                  color = False,
                  debug = True,
                  outbase = "psr_example_output")

