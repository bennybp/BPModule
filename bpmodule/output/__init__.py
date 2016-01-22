from .modinfo import *

# SO file
from .output import *

# for output:
# package args into a list then pass to SO file
def Output(s, *args):
    if(len(args)) > 0:
      Output_(s, [ str(x) for x in list(args) ] )
    else:
      Output_(s, [])


def Success(s, *args):
    if(len(args)) > 0:
      Success_(s, [ str(x) for x in list(args) ] )
    else:
      Success_(s, [])


def Warning(s, *args):
    if(len(args)) > 0:
      Warning_(s, [ str(x) for x in list(args) ] )
    else:
      Warning_(s, [])


def Error(s, *args):
    if(len(args)) > 0:
      Error_(s, [ str(x) for x in list(args) ] )
    else:
      Error_(s, [])


def Debug(s, *args):
    if(len(args)) > 0:
      Debug_(s, [ str(x) for x in list(args) ] )
    else:
      Debug_(s, [])


def Changed(s, *args):
    if(len(args)) > 0:
      Changed_(s, [ str(x) for x in list(args) ] )
    else:
      Changed_(s, [])

