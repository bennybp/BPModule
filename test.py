#!/usr/bin/env python3

import argparse
import ctypes
import os

parser = argparse.ArgumentParser(description="Testing")
parser.add_argument("sofile", help="Path to store so file")
parser.add_argument("modfile", help="Path to module directory")
args = parser.parse_args()


if not os.path.isfile(args.sofile):
  raise RuntimeError("SO File does not exist!")

mstore = ctypes.CDLL(args.sofile)

flist = [ x.strip() for x in open(args.modfile).readlines() ]
sofiles = [ x for x in flist if x.endswith(".so") and not x.startswith('#') ]

loader = mstore.LoadModule
loader.restype = ctypes.c_bool

# Check all files first
for s in sofiles:
  fullpath = os.path.abspath(s);
  
  if not os.path.isfile(fullpath):
    raise RuntimeError("SO module doesn't exist or is not readable: '{}'".format(fullpath)) 

for s in sofiles:
  fullpath = os.path.abspath(s);
  stbuf = ctypes.create_string_buffer(fullpath.encode('ascii', 'ignore'))
  stbuf2 = ctypes.create_string_buffer("".encode('ascii', 'ignore'))
  if mstore.LoadModule(stbuf, stbuf2) == False:
    raise RuntimeError("Unable to load modules. Aborting...")

mstore.DumpInfo()
