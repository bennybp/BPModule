from .modinfo import *

# SO file
from .util import *

def FormatStr(s, *args):
    if(len(args)) > 0:
      return FormatString_(s, list(args))
    else:
      return FormatString_(s, [])





def Line(char, n = 80):
    return char * n + "\n" 
