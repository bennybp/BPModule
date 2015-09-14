#!/usr/bin/env python3


############################
# Testing of ModuleInfo
############################

import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def RemoveKey(m, key):
  m2 = copy.deepcopy(m)
  m2.pop(key, None)
  return m2


def ReplaceKey(m, key, val):
  m2 = copy.deepcopy(m)
  m2[key] = val;
  return m2



def Run():
    try:
        bp.testing.PrintHeader("Testing construction of ModuleInfo")

        # This one should work
        # Note that we are not passing options
        minfo = {
            "key"         : "TESTKEY",
            "name"        : "TESTNAME",
            "path"        : "TESTPATH",
            "type"        : "core",
            "version"     : "0.1a",
            "description" : "Testing of core functionality",
            "authors"     : [],
            "refs"        : [],
        }

        nfailed = 0
        ntest = 1

        nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Acceptible ModuleInfo",        True,  minfo)
        ntest += 1

        nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Empty ModuleInfo",             False, {})
        ntest += 1

        # test missing elements
        for k in sorted(minfo.keys()):
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Missing {}".format(k),    False, RemoveKey(minfo, k))
            ntest += 1

        


        bp.testing.PrintResults(nfailed) 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      #traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
