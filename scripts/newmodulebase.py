#!/usr/bin/env python3

import os
import argparse
import shutil
import bp_common


thispath = os.path.dirname(os.path.realpath(__file__))
modpath = os.path.join(os.path.dirname(thispath), "bpmodule", "modulebase")

parser = argparse.ArgumentParser()
parser.add_argument("--author", required=True, help="Author of the file")
parser.add_argument("--desc", required=True, help="Short description of the base module")
parser.add_argument("name", help="Name of the module class")
args = parser.parse_args()


hfilename = args.name + ".hpp"
cfilename = args.name + ".cpp"
hfilepath = os.path.join(modpath, hfilename)
cfilepath = os.path.join(modpath, cfilename)

htemplatepath = os.path.join(modpath, "NewModule.hpp.template")
ctemplatepath = os.path.join(modpath, "NewModule.cpp.template")


hguard = bp_common.GenIncludeGuard(hfilename)

print("Creating {}".format(hfilepath))
with open(hfilepath, 'w') as dest:
    for l in open(htemplatepath, 'r').readlines():
      l = l.replace("AUTHOR", args.author)
      l = l.replace("MODULEDESC", args.desc)
      l = l.replace("CLASSNAME", args.name)
      l = l.replace("HEADERGUARD", hguard)
      dest.write(l)
