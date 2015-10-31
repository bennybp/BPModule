#!/usr/bin/env python3

import os
import sys
import re
import bp_common 


if len(sys.argv) != 2:
   print("Usage: generate_atommass.py outbase")
   quit(1)


outbase = sys.argv[1]
datadir = "data"

atominfo = {}


for l in open(os.path.join(datadir, "ElementNames.txt")).readlines():
    l.strip()
    z, sym, name = l.split()

    atominfo[z] = { "sym" : sym,
                    "name" : name,
                    "isos" : {}
                  }


# Read in experimental masses
for l in open(os.path.join(datadir, "CIAAW-MASSES.formatted.txt")).readlines()[5:]:
  l.strip()
  z, sym, low, high = l.split()
  atominfo[z]["mass"] = ( low, high )



# Read in isotop masses
for l in open(os.path.join(datadir, "CIAAW-ISOTOPEMASSES.formatted.txt")).readlines()[5:]:
  l.strip()
  z, sym, isonum, low, high = l.split()

  if not isonum in atominfo[z]["isos"]:
      atominfo[z]["isos"][isonum] = {}

  atominfo[z]["isos"][isonum]["mass"] = (low, high)


# Read in isotope abundances
for l in open(os.path.join(datadir, "CIAAW-ABUNDANCE.formatted.txt")).readlines()[5:]:
  l.strip()
  z, sym, isonum, low, high = l.split()

  if not isonum in atominfo[z]["isos"]:
      atominfo[z]["isos"][isonum] = {}

  atominfo[z]["isos"][isonum]["abundance"] = (low, high)


# Fill in missing isotope info?
for z, atom in atominfo.items():
  for isonum, isodat in atom["isos"].items():
    if not "mass" in isodat:
        isodat["mass"] = (isonum, isonum)
    if not "abundance" in isodat:
        isodat["abundance"] = (0, 0)



for z, atom in atominfo.items():
  print("{:<4}  {:<4}  {:<}".format(z, atom["sym"], atom["name"]))
  print("     Mass: ( {:<16} , {:<16} )".format(atom["mass"][0], atom["mass"][1]))

  for isonum, isodat in atom["isos"].items():
    print("   Isotope: {:<4}".format(isonum))
    print("             Mass: ( {:<16} , {:<16} )".format(isodat["mass"][0], isodat["mass"][1]))
    print("        Abundance: ( {:<16} , {:<16} )".format(isodat["abundance"][0], isodat["abundance"][1]))
  
  print()

quit()

with bp_common.HeaderSourceFiles(outbase, "LUT for Atomic Masses", 
                                 ["math", "lut"],
                                 createheader = False,
                                 hppincludes = [],
                                 cppincludes = ["<map>"]) as src:

    src.f.write("std::map<int, std::map<int, double>> atom_mass_map_{\n")
    for i in range(0, 200):
      if i in atoms:
        atom = atoms[i]
    
        src.f.write('    {{  {:5<} ,  {{    // {}\n'.format(i, atom['name']))
        for iso in atom['isos']:
             src.f.write('                {{ {:5<} , {:<} }},\n'.format(iso[0], iso[1]))
        src.f.write('            },\n')
        src.f.write('    },\n')
    src.f.write('};\n')

