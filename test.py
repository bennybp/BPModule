#!/usr/bin/env python3

import argparse
import ctypes
import os

parser = argparse.ArgumentParser(description="Testing")
parser.add_argument("sofile", help="Path to store so file")
parser.add_argument("moddir", help="Path to module directory")
args = parser.parse_args()


if not os.path.isfile(args.sofile):
  raise RuntimeError("SO File does not exist!")

mstore = ctypes.CDLL(args.sofile)

sofiles = [ x for x in os.listdir(args.moddir) if x.endswith(".so") ]

loader = mstore.LoadModule
loader.restype = ctypes.c_bool

for s in sofiles:
  print("PYTHON: loading " + s);
  fullpath = os.path.abspath(os.path.join(args.moddir, s));
  stbuf = ctypes.create_string_buffer(fullpath.encode('ascii', 'ignore'))
  stbuf2 = ctypes.create_string_buffer("".encode('ascii', 'ignore'))
  if mstore.LoadModule(stbuf, stbuf2) == False:
    raise RuntimeError("Unable to load modules. Aborting...")

mstore.DumpInfo()
