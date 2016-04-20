#!/usr/bin/env python3

import os
import sys
import re
import bp_common 


if len(sys.argv) != 2:
   print("Usage: generate_atommass.py outbase")
   quit(1)


outbase = sys.argv[1]
mypath = os.path.dirname(os.path.realpath(__file__))
datadir = os.path.join(mypath, "data")

atomicinfo = {}


for l in open(os.path.join(datadir, "ElementNames.txt")).readlines():
    l.strip()
    z, sym, name = l.split()
    z = int(z)

    atomicinfo[z] = { "sym" : sym,
                      "name" : name,
                      "mult": 0,
                      "termsym": "x",
                      "mass": (0.0, 0.0, 0.0),
                      "covradius":0.0,
                      "vdwradius":0.0,
                      "isos" : {}
                    }


# Read in experimental masses
for l in open(os.path.join(datadir, "CIAAW-MASSES.formatted.txt")).readlines()[5:]:
  l.strip()
  z, sym, mid, low, high = l.split()
  z = int(z)

  atomicinfo[z]["mass"] = ( mid, low, high )

#Read in Covalent Radii
for l in open(os.path.join(datadir,"CovRadii.txt")).readlines()[1:]:
  l.strip()
  z,r,unit=l.split()
  z=int(z)
  r=float(r)
  atomicinfo[z]["covradius"]=r/52.917721067 #conversion from generate_constants

#Read in van der waal Radii
for l in open(os.path.join(datadir,"VanDerWaalRadius.txt")).readlines()[1:]:
  l.strip()
  z,r,unit=l.split()
  z=int(z)
  r=float(r)
  atomicinfo[z]["vdwradius"]=r/52.917721067 #conversion from generate_constants


# Read in isotop masses
for l in open(os.path.join(datadir, "CIAAW-ISOTOPEMASSES.formatted.txt")).readlines()[5:]:
  l.strip()
  z, sym, isonum, mid, low, high = l.split()
  z = int(z)

  if not isonum in atomicinfo[z]["isos"]:
      atomicinfo[z]["isos"][isonum] = { }

  atomicinfo[z]["isos"][isonum]["mass"] = (mid, low, high)


# Read in isotope abundances
for l in open(os.path.join(datadir, "CIAAW-ABUNDANCE.formatted.txt")).readlines()[5:]:
  l.strip()
  z, sym, isonum, mid, low, high = l.split()
  z = int(z)

  if not isonum in atomicinfo[z]["isos"]:
      atomicinfo[z]["isos"][isonum] = { }

  atomicinfo[z]["isos"][isonum]["abundance"] = (mid, low, high)


# Fill in missing isotope info?
for z, atom in atomicinfo.items():
  for isonum, isodat in atom["isos"].items():
    if not "mass" in isodat:
        isodat["mass"] = (isonum, isonum, isonum)
    if not "abundance" in isodat:
        isodat["abundance"] = (0, 0, 0)


# Read in multiplicities
for l in open(os.path.join(datadir, "NIST-ATOMICION.formatted.txt")).readlines()[5:]:
  z, occ, mult, termsym = l.split()
  z = int(z)
  atomicinfo[z]["mult"] = int(mult)
  atomicinfo[z]["termsym"] = termsym



with bp_common.HeaderSourceFiles(outbase, "LUTs for Atomic Information", 
                                 ["bpmodule", "system", "lut"],
                                 hppincludes = ["<map>",
                                                "\"bpmodule/system/AtomicInfo.hpp\"",
                                                "\"bpmodule/util/StringUtil.hpp\""],
                                 cppincludes = ["<map>",
                                                "\"bpmodule/system/AtomicInfo.hpp\"",
                                                "\"bpmodule/util/StringUtil.hpp\""]) as src:

    # Header file
    src.fh.write("//////////////////////////////////////////////////\n")
    src.fh.write("// Various data maps. Stored in LUT source file\n")
    src.fh.write("//////////////////////////////////////////////////\n")
    src.fh.write("extern const std::map<int, std::string> atomic_Z_sym_;\n\n")
    src.fh.write("extern const std::map<std::string, int, util::CaseInsensitiveLess> atomic_sym_Z_;\n\n")
    src.fh.write("extern const std::map<int, AtomicData> atomic_Z_data_;\n\n")

    # Now the source file
    # First, atomic Z to symbol map
    src.f.write("extern const std::map<int, std::string> atomic_Z_sym_{\n")
    for k,v in sorted(atomicinfo.items()):
        src.f.write("  {{ {} , \"{}\" }},\n".format(k, v["sym"]))
    src.f.write("}; // close atomic_Z_sym_\n\n\n")

    # Next, atomic symbol to Z
    src.f.write("extern const std::map<std::string, int, util::CaseInsensitiveLess> atomic_sym_Z_{\n")
    for k,v in sorted(atomicinfo.items()):
        src.f.write("  {{ \"{}\" , {} }},\n".format(v["sym"], k))
    src.f.write("}; // close atomic_sym_Z_\n\n\n")

    # Next, full atomic data
    src.f.write("extern const std::map<int, AtomicData> atomic_Z_data_{\n")
    for k,v in sorted(atomicinfo.items()):
        src.f.write("  {{ {:<4} , {{ {},\n".format(k, k))
        src.f.write("             \"{}\",\n".format(v["sym"]))
        src.f.write("             \"{}\",\n".format(v["name"]))
        src.f.write("             {},\n".format(v["mult"]))
        src.f.write("             \"{}\",\n".format(v["termsym"]))
        src.f.write("             {},\n".format(v["mass"][0]))
        src.f.write("             {},\n".format(v["mass"][1]))
        src.f.write("             {},\n".format(v["mass"][2]))
        src.f.write("             {},\n".format(v["covradius"]))
        src.f.write("             {},\n".format(v["vdwradius"]))
        # isotope info
        src.f.write("              {\n")
        for ki,vi in sorted(v["isos"].items()):
            src.f.write("                 {{ {}, {}, {}, {}, {}, {}, {} }},\n".format(ki,
                                                                                      vi["mass"][0],
                                                                                      vi["mass"][1],
                                                                                      vi["mass"][2],
                                                                                      vi["abundance"][0],
                                                                                      vi["abundance"][1],
                                                                                      vi["abundance"][2]))
        src.f.write("              },\n")  # Closes isotope vector

        src.f.write("           },\n")  # Closes atomic data
        src.f.write("  },\n\n")  # closes map pair
    src.f.write("}; // close atomic_Z_data_\n\n\n")




    
