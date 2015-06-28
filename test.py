#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp

bp.Init()

# Set some outputs
bp.bpcore.SetOut_Stdout()
#bpcore.SetOut_File("test.out")
bp.bpcore.SetColor(True)
bp.bpcore.SetDebug(True)


bp.Output("******************************\n")
bp.Output("* Testing output             *\n")
bp.Output("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Success("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Warning("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Error("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Debug("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)

# Load the python modules
bp.LoadModules(
               [
                "testmodule1",
                "testpymodule1"
               ]
              )

# Test
try:
  b1 = bp.mst.GetModule_Test("TESTMOD1")
  b2 = bp.mst.GetModule_Test("TESTPYMOD1")
  print(b2)

  b1.RunTest()
  print()
  b1.CallRunTest("TESTMOD1")
  print()
  b1.CallRunTest("TESTPYMOD1")
  print()

  b2.RunTest()
  print()
  b2.CallRunTest("TESTMOD1")
  print()
  b2.CallRunTest("TESTPYMOD1")
  print()

  print("\n!!!Testing exceptions\n")
  try:
    b1.Throw()
  except RuntimeError as e2:
    bp.Error(str(e2))

  print()

  try:
    b2.Throw()
  except RuntimeError as e2:
    bp.Error(str(e2))

  print()

  try:
    b1.CallThrow("TESTMOD1")
  except RuntimeError as e2:
    bp.Error(str(e2))

  print()

  try:
    b2.CallThrow("TESTPYMOD1")
  except RuntimeError as e2:
    bp.Error(str(e2))

  try:
    b2.CallThrow("TESTMOD1")
  except RuntimeError as e2:
    bp.Error(str(e2))

  print()

  try:
    b1.CallThrow("TESTMOD1")
  except RuntimeError as e2:
    bp.Error(str(e2))

  print()


  print("\nDone testing\n")

except RuntimeError as e:
  print("Caught exception in main handler")
  traceback.print_exc()
  print(e)


bp.Finalize()
