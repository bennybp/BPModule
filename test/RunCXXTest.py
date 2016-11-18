import importlib
import sys
from TestFxns import *

test_name = sys.argv[1]
m = importlib.import_module(test_name)
m.run_test()
