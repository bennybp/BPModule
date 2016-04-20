from .modinfo import *

# SO file
from .testing import *

# Tester class
from .Tester import *

from pulsar.output import GlobalOutput, GlobalError, GlobalSuccess, GlobalDebug

##################
# For testing on the python side
##################
def PyTestFunc(func, *args):
    try:
       func(*args)
    except Exception as e:
        GlobalDebug(str(e) + "\n")
        return 0
    except:
        return 0

    return 1



def PyTestBoolFunc(func, *args):
    try:
        return func(*args)

    except Exception as e:
        GlobalDebug(str(e) + "\n")
        return 0

    except:
        return 0

