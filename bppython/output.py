import sys
import os

from bppython import *


# for output:
# package args into a list then pass to bpbase
def Output(s, *args):
    if(len(args)) > 0:
      bpbase.Output(s, list(args))
    else:
      bpbase.Output(s, [])


def Success(s, *args):
    if(len(args)) > 0:
      bpbase.Success(s, list(args))
    else:
      bpbase.Success(s, [])


def Warning(s, *args):
    if(len(args)) > 0:
      bpbase.Warning(s, list(args))
    else:
      bpbase.Warning(s, [])


def Error(s, *args):
    if(len(args)) > 0:
      bpbase.Error(s, list(args))
    else:
      bpbase.Error(s, [])


def Debug(s, *args):
    if(len(args)) > 0:
      bpbase.Debug(s, list(args))
    else:
      bpbase.Debug(s, [])


def Changed(s, *args):
    if(len(args)) > 0:
      bpbase.Changed(s, list(args))
    else:
      bpbase.Changed(s, [])


def Flush():
    bpbase.Flush()


def Line(char, n = 80):
    return char * n + "\n" 


