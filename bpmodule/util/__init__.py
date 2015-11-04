from .modinfo import *

# SO file
from .util import FormatStr as FormatStr_

def FormatStr(s, *args):
    if(len(args)) > 0:
      return FormatStr_(s, list(args))
    else:
      return FormatStr_(s, [])





def Line(char, n = 80):
    return char * n + "\n" 
