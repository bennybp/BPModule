from .modinfo import *

# SO file
from .output import *

# for output:
# package args into a list then pass to SO file
def GlobalOutput(s, *args):
    if(len(args)) > 0:
      GlobalOutput_(s, [ str(x) for x in list(args) ] )
    else:
      GlobalOutput_(s, [])


def GlobalSuccess(s, *args):
    if(len(args)) > 0:
      GlobalSuccess_(s, [ str(x) for x in list(args) ] )
    else:
      GlobalSuccess_(s, [])


def GlobalWarning(s, *args):
    if(len(args)) > 0:
      GlobalWarning_(s, [ str(x) for x in list(args) ] )
    else:
      GlobalWarning_(s, [])


def GlobalError(s, *args):
    if(len(args)) > 0:
      GlobalError_(s, [ str(x) for x in list(args) ] )
    else:
      GlobalError_(s, [])


def GlobalDebug(s, *args):
    if(len(args)) > 0:
      GlobalDebug_(s, [ str(x) for x in list(args) ] )
    else:
      GlobalDebug_(s, [])


def GlobalChanged(s, *args):
    if(len(args)) > 0:
      GlobalChanged_(s, [ str(x) for x in list(args) ] )
    else:
      GlobalChanged_(s, [])

