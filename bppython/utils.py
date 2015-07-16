import sys
import os

from bppython import *


# for output:
# package args into a list then pass to bpcore
def Output(s, *args):
    if(len(args)) > 0:
      bpcore.Output(s, list(args))
    else:
      bpcore.Output(s, [])


def Success(s, *args):
    if(len(args)) > 0:
      bpcore.Success(s, list(args))
    else:
      bpcore.Success(s, [])


def Warning(s, *args):
    if(len(args)) > 0:
      bpcore.Warning(s, list(args))
    else:
      bpcore.Warning(s, [])


def Error(s, *args):
    if(len(args)) > 0:
      bpcore.Error(s, list(args))
    else:
      bpcore.Error(s, [])


def Debug(s, *args):
    if(len(args)) > 0:
      bpcore.Debug(s, list(args))
    else:
      bpcore.Debug(s, [])


def Changed(s, *args):
    if(len(args)) > 0:
      bpcore.Changed(s, list(args))
    else:
      bpcore.Changed(s, [])


def Flush():
    bpcore.Flush()


def Line(char, n = 80):
    return char * n + "\n" 



def PrintModuleInfo(key, minfo):

  Output("\n")
  Output("  ++ Module: %1%\n", key)
  Output("             Key: %1%\n", minfo["key"])
  Output("            Name: %1%\n", minfo["name"])
  Output("            Type: %1%\n", minfo["type"])
  Output("            Path: %1%\n", minfo["path"])

  if "soname" in minfo:
      Output("          SOName: %1%\n", minfo["soname"])

  Output("         Version: %1%\n", minfo["version"])
  Output("     Description: %1%\n", minfo["description"])

  Output("         Authors: %1%\n", len(minfo["authors"]))
  for aut in minfo["authors"]:
      Output("                  %1%\n", aut)

  Output("      References: %1%\n", len(minfo["refs"]))
  for ref in minfo["refs"]:
      Output("                  %1%\n", ref)

  if "passedoptions" in minfo:
      nopt = len(minfo["passedoptions"])
      Output("         Options: %1%\n", nopt)

      if nopt > 0:
          Output("\n")
          l = "-"*20
          Output("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", "Option", "Default", "Value", "Description")
          Output("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", l, l, l, l)

          defopt = minfo["options"]
          for opt in minfo["passedoptions"]:
              d = defopt[opt[0]]
              help = d[3]

              if type(d[0]) == type:
                dv = "({})".format(d[0].__name__)
              elif type(d[0]) == list and type(d[0][0]) == type:
                dv = "[ ({}) ]".format(d[0][0].__name__)
              else:
                dv = d[0]

              # if not the same, highlight change
              if dv != opt[1]:
                  OutFunc = Changed
              else:
                  OutFunc = Output
      
              if type(opt[1]) == list:  # note - zero elements not allowed
                if type(dv) == list:
                  nel = max(len(opt[1]), len(dv))
                else:
                  nel = len(opt[1])


                # first row
                if type(dv) == list:
                  OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", opt[0], dv[0], opt[1][0], help)
                else: 
                  OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", opt[0], dv, opt[1][0], help)

                for i in range(1, nel):
                  dstr = dv[i] if (type(dv) == list and i < len(dv)) else ""
                  vstr = opt[1][i] if (i < len(opt[1])) else ""

                  OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", "", dstr, vstr, "")
        
              else:        
                OutFunc("      %|1$-20|      %|2$-20|      %|3$-20|     %4%\n", opt[0], dv, opt[1], help)

  else:
      Output("         Options: NOT YET SET\n")

  Output("\n")

