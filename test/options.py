#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp


def ShouldFail(key, opt):
  try:
    bp.SetOptions(key, opt)
  except Exception as e:
    bp.Output("\n")
    bp.Error(str(e))
    bp.Output("\n")
    #traceback.print_exc()
    print("Test that should have failed did fail")
    return True

  print("Test that should have failed did NOT fail")
  return False


def ShouldSucceed(key, opt):
  try:
    bp.SetOptions(key, opt)
  except Exception as e:
    bp.Output("\n")
    bp.Error(str(e))
    bp.Output("\n")
    #traceback.print_exc()
    print("Test that should have succeeded did NOT succeed")
    return False

  print("Test that should have succeeded did succeed")
  return True



bp.Init(debug = True, output="/tmp/test.out")


try:

  bp.LoadModule("test_options", "TEST_INT")
  bp.LoadModule("test_options", "TEST_FLOAT")

  ###################################
  # Test loading with various options
  # Including successes and failures
  ###################################
  ShouldSucceed("TEST_INT", { 'int_req': 100 } )
  ShouldFail("TEST_INT", { 'int_req' : "Hi" })
  ShouldFail("TEST_INT", { 'int_req' : [10] })
  ShouldFail("TEST_INT", { 'int_req' : 5.0 })
  ShouldFail("TEST_INT", {})
  ShouldFail("TEST_INT", { 'int_opt' : "Hi" })
  ShouldFail("TEST_INT", { 'int_opt' : [10] })
  ShouldFail("TEST_INT", { 'int_opt' : 5.0 })
  ShouldFail("TEST_INT", { 'int_opt_def' : "Hi" })
  ShouldFail("TEST_INT", { 'int_opt_def' : [10] })
  ShouldFail("TEST_INT", { 'int_opt_def' : 5.0 })
  ShouldFail("TEST_INT", { 'int_not_exist' : 5 })

  ShouldSucceed("TEST_FLOAT", { 'float_req': 10.0 } )
  ShouldFail("TEST_FLOAT", { 'float_req' : "Hi" })
  ShouldFail("TEST_FLOAT", { 'float_req' : [10.0] })
  ShouldFail("TEST_FLOAT", { 'float_req' : 5 })
  ShouldFail("TEST_FLOAT", {})
  ShouldFail("TEST_FLOAT", { 'float_opt' : "Hi" })
  ShouldFail("TEST_FLOAT", { 'float_opt' : [10.0] })
  ShouldFail("TEST_FLOAT", { 'float_opt' : 5 })
  ShouldFail("TEST_FLOAT", { 'float_opt_def' : "Hi" })
  ShouldFail("TEST_FLOAT", { 'float_opt_def' : [10.0] })
  ShouldFail("TEST_FLOAT", { 'float_opt_def' : 5.0 })
  ShouldFail("TEST_FLOAT", { 'float_not_exist' : 5.0 })




except Exception as e:
  bp.Output("\n\n")
  bp.Error("***Caught exception in main handler\n")
  traceback.print_exc()
  bp.Error("\n")
  bp.Error(str(e))
  bp.Error("\n")


bp.Finalize()