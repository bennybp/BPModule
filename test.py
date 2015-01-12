#!/usr/bin/env python3

import argparse
import ctypes
import os

parser = argparse.ArgumentParser(description="Testing")
parser.add_argument("pyso", help="Path to python loader so file")
parser.add_argument("sofiles", help="Path to file with list of core so files")
parser.add_argument("mainso", help="Path to main python so file")
parser.add_argument("modfiles", help="Path to file with list of module so files")
args = parser.parse_args()


def MakeCStr(s):
  return ctypes.create_string_buffer(s.encode('ascii', 'ignore'))


# Load bootstrap loader
pyload = ctypes.CDLL(args.pyso)
initer = pyload.PythonLoader
initer.restype = ctypes.c_bool

# Initialize with core sos first
flist = [ x.strip() for x in open(args.sofiles).readlines() ]
sofiless = [ x for x in flist if x.endswith(".so") and not x.startswith('#') ]

for s in sofiless:
  fullpath = os.path.abspath(s);
  if not os.path.isfile(fullpath):
    raise RuntimeError("SO module doesn't exist or is not readable: '{}'".format(fullpath)) 

for s in sofiless:
  fullpath = os.path.abspath(s);
  print("Loading core so: {}".format(fullpath))
  stbuf = MakeCStr(fullpath)
  if initer(stbuf) == False:
    raise RuntimeError("Unable to load modules. Aborting...")


# Now the main python module, since the dependencies are loaded
mstore = ctypes.CDLL(args.mainso)
loader = mstore.LoadSO
loader.restype = ctypes.c_bool


# Now the rest of the modules
flist = [ x.strip() for x in open(args.modfiles).readlines() ]
sofiless = [ x for x in flist if x.endswith(".so") and not x.startswith('#') ]


# Check all files first
for s in sofiless:
  fullpath = os.path.abspath(s);
  
  if not os.path.isfile(fullpath):
    raise RuntimeError("SO module doesn't exist or is not readable: '{}'".format(fullpath)) 

for s in sofiless:
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
print("=====END=====")

print("Finalizing parallel module")
mstore.FinalizeParallel();
