#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp


def ShouldFail(supermodule, modkey, opt):
  try:
    bp.LoadModule(supermodule, modkey, opt, test = True)
  except Exception as e:
    bp.Output("\n")
    bp.Error(str(e))
    bp.Output("\n")
    #traceback.print_exc()
    print("Module that should have failed did fail")
    return True

  print("Module that should have failed did NOT fail")
  return False


def ShouldSucceed(supermodule, modkey, opt):
  try:
    bp.LoadModule(supermodule, modkey, opt, test = True)
  except Exception as e:
    bp.Output("\n")
    bp.Error(str(e))
    bp.Output("\n")
    #traceback.print_exc()
    print("Module that should have succeeded did NOT succeed")
    return False

  print("Module that should have succeeded did succeed")
  return True



bp.Init(debug = True, output="/tmp/test.out")


try:

  ###################################
  # Test loading with various options
  # Including successes and failures
  ###################################
  ShouldSucceed("test_options", "TEST_INT", { 'int_req': 100 } );
  ShouldFail("test_options", "TEST_INT", { 'int_req' : "Hi" })
  ShouldFail("test_options", "TEST_INT", {})



except Exception as e:
  bp.Output("\n\n")
  bp.Error("***Caught exception in main handler\n")
  traceback.print_exc()
  bp.Error(str(e))
  bp.Output("\n\n")


bp.Finalize()
