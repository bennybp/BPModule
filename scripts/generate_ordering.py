#!/usr/bin/env python3

import sys
import bp_common


if len(sys.argv) != 3:
   print("Usage: generate_ordering.py maxam outbase")
   quit(1)

outbase = sys.argv[2]



def IterateCartesian(g, am):
  if g[2] == am:
    newg = (-1, -1, -1)
  elif g[2] < (am - g[0]):
    newg = (g[0], g[1]-1, g[2]+1)
  else:
    newg = (g[0]-1, am-g[0]+1, 0)

  return newg

def IsValid(g, am):
  return (g[0] >= 0 and g[0] <= am and
          g[1] >= 0 and g[1] <= am and
          g[2] >= 0 and g[2] <= am and
          g[0] + g[1] + g[2] == am)



maxl = int(sys.argv[1])

cartmap = {}
sphmap = {}

for l in range(0, maxl+1):
    sphmap[l] = [ x for x in range(-l, l+1) ]

    g = (l, 0, 0)

    cartmap[l] = []

    while(IsValid(g, l)):
      cartmap[l].append(g)
      g = IterateCartesian(g, l)
    

# do "negative" am (combined general contrations)
for l in range(-1, -5, -1):
    cartmap[l] = list(cartmap[l+1])
    sphmap[l] = list(sphmap[l+1])
    cartmap[l].extend(cartmap[abs(l)])
    sphmap[l].extend(sphmap[abs(l)])
    


with bp_common.HeaderSourceFiles(outbase, "Ordering of basis functions",
                                 ["bpmodule","system","lut"],
                                 createheader = False,
                                 srcincludes = ["\"bpmodule/system/Ordering.hpp\""]) as src:

    src.f.write("//Map of AM to exponents on x, y, z\n")
    src.f.write("extern const CartesianOrderingMap ao_cartesian_order_{\n");

    for l in range(-4, maxl+1):
        src.f.write("  {{ {:3}, {{\n".format(l))
        for v in cartmap[l]:
            src.f.write("          {{ {:3},{:3},{:3} }},\n".format(v[0], v[1], v[2]));
        src.f.write("         },\n")
        src.f.write("  },\n")

    src.f.write("};\n")

    src.f.write("\n\n")
    src.f.write("//Map of AM to m_l value\n")
    src.f.write("extern const SphericalOrderingMap ao_spherical_order_{\n");

    for l in range(-4, maxl+1):
        src.f.write("{{ {:3}, {{".format(l))
        for v in sphmap[l]:
            src.f.write("{:3},".format(v))
        src.f.write("}  },\n")

    src.f.write("};\n")
