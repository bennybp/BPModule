#!/usr/bin/env python3

###################################
# Fixes the header guard for a file
###################################
import sys
import os

import psr_common


for f in sys.argv[1:]:
  if not os.path.isfile(f):
    print("File doesn't exist or is not a file: {}".format(f)) 
    continue

  guard = psr_common.GenIncludeGuard(f)

  print("Processing file {}".format(f)) 
  print("  Header guard: {}".format(guard)) 
  


  lines = open(f).readlines()

  found = False
  inifndef = False

  # Just output all the lines, stopping at the first ifndef
  with open(f, 'w') as new:
    for l in lines:
      if l.startswith("#ifndef"):
        found = True
        inifndef = True
        new.write("#ifndef {}\n".format(guard))
      elif l.startswith("#define") and inifndef:
        inifndef = False
        new.write("#define {}\n".format(guard))
      else:
        new.write(l)

  if not found:
    print("    WARNING: Existing header guard not found")
