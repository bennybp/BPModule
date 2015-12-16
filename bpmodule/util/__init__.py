from .modinfo import *

# SO file
from .util import *

def FormatStr(s, *args):
    if(len(args)) > 0:
      # Should automatically convert to vector of strings
      # or error if an arg can't be converted to a string
      return FormatString_(s, [ str(x) for x in list(args) ])
    else:
      return FormatString_(s, [])





def Line(char, n = 80):
    return char * n + "\n" 
