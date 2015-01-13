#!/usr/bin/env python3

import argparse
import ctypes
import os
import importlib
import sys

parser = argparse.ArgumentParser(description="Testing")
parser.add_argument("pyso", help="Path to python loader so file")
parser.add_argument("sofiles", help="Path to file with list of core so files")
parser.add_argument("mainso", help="Path to main python so file")
parser.add_argument("modfiles", help="Path to file with list of module so files")
args = parser.parse_args()

def MakeCStr(s):
  return ctypes.create_string_buffer(s.encode('ascii', 'ignore'))

def MakeCStrVec(s):
  vec = (ctypes.c_char_p * len(s))()
  idx = 0
  for a in [ x.strip() for x in s]:
    vec[idx] = bytes(str(a), 'ascii')
    idx += 1
  return vec

def MakeCOpts(s):
  vec = ((ctypes.c_char_p * 3)*len(s))()
  idx = 0
  for a in s:
    print(a)
    vec[idx][0] = bytes(str(a[0]), 'ascii')
    vec[idx][1] = bytes(str(a[1]), 'ascii')
    vec[idx][2] = bytes(str(a[2]), 'ascii')
    idx += 1
  return vec
  

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
pyfiles = [ x for x in flist if not x.startswith('#') ]

# Import python modules
for s in pyfiles:
  fullpath = os.path.abspath(s);
  mod1, mod2 = os.path.split(fullpath)
  sys.path.append(mod1)
  f = importlib.import_module(mod2 + ".modinfo")

  # parse out the module info
  for k,v in f.compinfo.items():
    mpath = MakeCStr(os.path.join(fullpath, k))
    mname = MakeCStr(v["name"])
    mversion = MakeCStr(v["version"])
    mdesc = MakeCStr(v["description"])
    mclass = MakeCStr(v["class"])
    mtype = MakeCStr(v["type"])
    mauth = MakeCStrVec(v["authors"])
    mrefs = MakeCStrVec(v["refs"])
    mopts = MakeCOpts(v["options"])

    if loader(mpath, mname, mversion, mclass, mtype, mdesc, 
              len(v["authors"]), mauth,
              len(v["refs"]), mrefs,
              0, mopts) == False:
              #len(v["options"]), mopts) == False:
      raise RuntimeError("Unable to load modules. Aborting...")
    
mstore.DumpInfo()


print("Initializing parallel module")
mstore.InitParallel();

print("====TESTS====")
mstore.RunTest(MakeCStr("TESTMOD1"))
print("=====END=====")

print("Finalizing parallel module")
mstore.FinalizeParallel();
