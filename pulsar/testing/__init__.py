# SO file
from ..pulsar_core.testing import *

# Tester class
from .Tester import *

from pulsar.output import *

##################
# For testing on the python side
##################
def py_test_function(func, *args):
    try:
       func(*args)
    except Exception as e:
        print_global_debug(str(e) + "\n")
        return 0
    except:
        return 0

    return 1



def py_test_bool_function(func, *args):
    try:
        return func(*args)

    except Exception as e:
        print_global_debug(str(e) + "\n")
        return 0

    except:
        return 0

