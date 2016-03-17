from .modinfo import *

# SO file
from .testing import *

# Tester class
from .tester import *

from bpmodule.output import GlobalOutput, GlobalError, GlobalSuccess, GlobalDebug

##################
# For testing on the python side
##################
def PyTestFunc(func, *args):
    try:
       func(*args)
    except Exception as e:
        GlobalDebug(str(e) + "\n")
        return 1
    except:
        return 1

    return 0



def PyTestBoolFunc(func, *args):
    try:
        if func(*args):
            return 0
        else:
            return 1

    except Exception as e:
        GlobalDebug(str(e) + "\n")
        return 0

    except:
        return 0

