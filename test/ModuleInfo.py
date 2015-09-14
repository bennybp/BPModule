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
            "authors"     : ["Hi", "There"],
            "refs"        : ["Some", "ref"],
        }

        nfailed = 0
        ntest = 1

        nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Acceptable ModuleInfo",        True,  minfo)
        ntest += 1

        nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Empty ModuleInfo",             False, {})
        ntest += 1

        allkeys = sorted(minfo.keys())
        strkeys = copy.deepcopy(allkeys)
        strkeys.remove("refs")
        strkeys.remove("authors")

        # test missing elements
        # test replacing keys with non-strings 
        for k in allkeys:
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Missing {}".format(k),                    False, RemoveKey(minfo, k)                      )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with int".format(k),           False, ReplaceKey(minfo, k, int(5) )            )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with float".format(k),         False, ReplaceKey(minfo, k, float(5) )          )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with dict".format(k),          False, ReplaceKey(minfo, k, {"hi": 4} )         )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with set".format(k),           False, ReplaceKey(minfo, k, {"hi", "4"} )       )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with tuple".format(k),         False, ReplaceKey(minfo, k, ("hi", 4) )         )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list int".format(k),      False, ReplaceKey(minfo, k, [int(5)] )          )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list float".format(k),    False, ReplaceKey(minfo, k, [float(5)] )        )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list dict".format(k),     False, ReplaceKey(minfo, k, [{"hi": 4}] )       )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list set".format(k),      False, ReplaceKey(minfo, k, [{"hi", "4"}] )     )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list tuple".format(k),    False, ReplaceKey(minfo, k, [("hi", 4)] )       )
            ntest += 1
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list list".format(k),     False, ReplaceKey(minfo, k, [[5, 4], [1, 0]] )  )
            ntest += 1

        # test replacing with a list of strings
        # (except those that are lists of strings)
        for k in strkeys:
            nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Replace {} with list str".format(k),  False, ReplaceKey(minfo, k, ["Hello", "Hi"] )       )
            ntest += 1


        nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Empty authors".format(k),  True, ReplaceKey(minfo, "authors", []))
        ntest += 1
        nfailed = nfailed + bp.testing.TestModuleInfo(ntest, "Empty refs".format(k),     True, ReplaceKey(minfo, "refs", []))
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
