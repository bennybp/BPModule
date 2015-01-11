#!/usr/bin/env python3

import argparse
import ctypes
import os

parser = argparse.ArgumentParser(description="Testing")
parser.add_argument("sofile", help="Path to store so file")
parser.add_argument("modfile", help="Path to module directory")
args = parser.parse_args()


def MakeCStr(s):
  return ctypes.create_string_buffer(s.encode('ascii', 'ignore'))


if not os.path.isfile(args.sofile):
  raise RuntimeError("SO File does not exist!")

mstore = ctypes.CDLL(args.sofile)

flist = [ x.strip() for x in open(args.modfile).readlines() ]
sofiles = [ x for x in flist if x.endswith(".so") and not x.startswith('#') ]

loader = mstore.LoadSO
loader.restype = ctypes.c_bool

# Check all files first
for s in sofiles:
  fullpath = os.path.abspath(s);
  
  if not os.path.isfile(fullpath):
    raise RuntimeError("SO module doesn't exist or is not readable: '{}'".format(fullpath)) 

for s in sofiles:
  fullpath = os.path.abspath(s);
  stbuf = MakeCStr(fullpath)
  stbuf2 = MakeCStr("")
  if loader(stbuf, stbuf2) == False:
    raise RuntimeError("Unable to load modules. Aborting...")

mstore.DumpInfo()


print("Initializing parallel module")
mstore.InitParallel();

print("====TESTS====")
mstore.RunTest(MakeCStr("TEST1"))
mstore.RunTest(MakeCStr("TEST2"))
mstore.RunTest(MakeCStr("TEST3"))
mstore.RunTest(MakeCStr("TEST4"))
print("=====END=====")

print("Finalizing parallel module")
mstore.FinalizeParallel();
