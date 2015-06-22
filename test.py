#!/usr/bin/env python3

import gc
import argparse
import os
import importlib
import sys

basepath = "/home/ben/programming/BPModule/install"
bppypath = os.path.join(basepath, "python")
modpath = os.path.join(basepath, "modules")


# import bpmodule python helpers
sys.path.append(bppypath)
import bpmodule as bp


# Load the core module
bpcore = bp.loader.LoadModule("bpmodule_core", modpath)

bpcore.SetOut_Stdout()
#bpcore.SetOut_File("test.out")
bpcore.SetColor(True)
bpcore.SetDebug(True)


# Load the python modules
t1 = bp.loader.LoadModule("testmodule1", modpath)


# Create the module store
mst = bpcore.ModuleStore()


# Load the so files
for key,minfo in t1.minfo.items():

  # find the full path of the so file
  fullpath = os.path.join(modpath, "testmodule1", minfo["soname"]) + ".so" 
  if not os.path.isfile(fullpath):
    raise RuntimeError("Error - {} does not exist or is not a file!".format(fullpath))

  print("  ++ Module: {}".format(key))
  print("            Path: {}".format(fullpath))
  print("            Name: {}".format(minfo["name"])) 
  print("          SOName: {}".format(minfo["soname"])) 
  print("         Version: {}".format(minfo["soname"])) 
  print("     Description: {}".format(minfo["description"])) 

  print("         Authors: {}".format(len(minfo["authors"]))) 
  for aut in minfo["authors"]:
      print("                  {}".format(aut))

  print("      References: {}".format(len(minfo["refs"]))) 
  for ref in minfo["refs"]:
      print("                  {}".format(ref))
 
  print("         Options: {}".format(len(minfo["options"]))) 
  for opt in minfo["options"]:
      print("                  {:<12}  :  {:<12}  : {}".format(opt[0], opt[1], opt[2]))

 
  mst.LoadSO(key, fullpath, minfo)
  #mst.Dump()


# Test
try:
  b1 = mst.GetModule_Test("TESTMOD1")
  #b1.RunTest()
  #print(mst.ModuleInfoFromKey("TESTMOD1"))
  #print(mst.KeyFromID(0))
  #print(mst.GetKeys())
  opt = b1.Options()
  inf = b1.Info()
 
except RuntimeError as e:
  print(e)


# manually destroy all objects? then the store
# A hack for now
b1 = None
gc.collect()
