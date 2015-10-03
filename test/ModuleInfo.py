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

        tester = bp.testing.Tester("Testing construction of ModuleInfo")
        tester.PrintHeader()

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
            "options"     : { }
        }


        tester.Test("Acceptable ModuleInfo", True, bp.testing.TestModuleInfo_Construct,  minfo)
        tester.Test("Empty ModuleInfo", False, bp.testing.TestModuleInfo_Construct,  {})

        allkeys = sorted(minfo.keys())
        strkeys = copy.deepcopy(allkeys)
        strkeys.remove("refs")
        strkeys.remove("authors")

        # test missing elements
        # test replacing keys with non-strings 
        for k in allkeys:
            tester.Test("Missing {}".format(k),                    False, bp.testing.TestModuleInfo_Construct, RemoveKey(minfo, k)                      )
            tester.Test("Replace {} with int".format(k),           False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, int(5) )            )
            tester.Test("Replace {} with float".format(k),         False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, float(5) )          )
            tester.Test("Replace {} with dict".format(k),          False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, {"hi": 4} )         )
            tester.Test("Replace {} with set".format(k),           False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, {"hi", "4"} )       )
            tester.Test("Replace {} with tuple".format(k),         False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, ("hi", 4) )         )
            tester.Test("Replace {} with list int".format(k),      False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, [int(5)] )          )
            tester.Test("Replace {} with list float".format(k),    False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, [float(5)] )        )
            tester.Test("Replace {} with list dict".format(k),     False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, [{"hi": 4}] )       )
            tester.Test("Replace {} with list set".format(k),      False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, [{"hi", "4"}] )     )
            tester.Test("Replace {} with list tuple".format(k),    False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, [("hi", 4)] )       )
            tester.Test("Replace {} with list list".format(k),     False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, [[5, 4], [1, 0]] )  )


        # test replacing with a list of strings
        # (except those that are lists of strings)
        for k in strkeys:
            tester.Test("Replace {} with list str".format(k),  False, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, k, ["Hello", "Hi"] )       )


        tester.Test("Empty authors".format(k),  True, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, "authors", []))
        tester.Test("Empty refs".format(k),     True, bp.testing.TestModuleInfo_Construct, ReplaceKey(minfo, "refs", []))


        tester.PrintResults()


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      #traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
