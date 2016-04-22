#!/usr/bin/env python3

import sys
from sympy import *
import psr_common


if len(sys.argv) != 3:
   print("Usage: generate_sphertransform.py maxam outbase")
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

# We have to store the AM with m_l to differentiate
# different m_l values in combined types
for l in range(0, maxl+1):
    sphmap[l] = [ (l,x) for x in range(-l, l+1) ]

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
    


########################################################
# Calculate Cartesian -> Spherical
# This algorithm is from Helgaker, Jorgensen, and Olsen
# "Molecular Electronic-Structure Theory
########################################################
tosph = {}
for l in range(0, maxl + 1):
  tosph[l] = {}

  for lm in sphmap[l]:
    l,m = lm

    tosph[l][lm] = {}
    absm = abs(m)
  
    d = (1 if m == 0 else 0)
    NlmS = 1 / (2**absm * factorial(l))
    NlmS *= sqrt( (2*factorial(l+absm)*factorial(l-absm)) / (2**d) )
  
  
    vm2 = (0 if m >= 0 else 1)
    vm  = sympify(vm2)/2
  
    onefourth = sympify("1/4")
  
    for t in range(0, (l-absm)//2 + 1):
      for u in range(0, t + 1):
        for v2 in range(vm2, int(2*(int(absm/2 - vm) + vm)) + 1, 2):
          v = sympify(v2)/2
          lxyz = ( int(2*t+absm-2*(u+v)), int(2*(u+v)), int(l-2*t-absm))
          c = ( (-1)**(t+v-vm) * onefourth**t * binomial(l, t) * binomial(l-t, absm + t) 
                * binomial(t, u) * binomial(absm, v2))
          tosph[l][lm][lxyz] = c*NlmS

# do negative am
for l in range(-4, 0):
    tosph[l] = {}
    for i in range(0, abs(l) + 1):
        tosph[l].update(tosph[i])
    
#print()
#print("Transformation to Spherical")
#for l in range(-4, maxl+1):
#    cartlist = cartmap[l]
#    sphlist = sphmap[l]
#
#    print()
#    print("l = {}".format(l))
#    for midx,lm in enumerate(sphlist):
#        print("    m = {:3}   (index {:3})".format(str(lm), midx))
#        for k,v in tosph[l][lm].items():
#            cartidx = cartlist.index(k)
#            print("        ({:2}, {:2}, {:2})    {:2} -> {:25}  =  {}".format(k[0], k[1], k[2], cartidx, v.evalf(20), v))
#        print()



with psr_common.HeaderSourceFiles(outbase, "Conversion from cartesian to spherical harmonics",
                                 ["pulsar","system","lut"],
                                 createheader = False,
                                 srcincludes = ["\"pulsar/system/SphericalTransform.hpp\""]) as src:

    src.f.write("extern const SphericalTransformMap spherical_transform_map_{\n")
    for l in range(-4, maxl+1):
        cartlist = cartmap[l]
        sphlist = sphmap[l]
        src.f.write("    {{ {:3} , // l = {}\n".format(l, l))
        src.f.write("      {\n")

        src.f.write("                 // {}  {}\n".format("midx", "cartidx"))
        for midx,lm in enumerate(sphlist):
            l2, m = lm
            for k,v in tosph[l][lm].items():
                cartidx = cartlist.index(k)
                src.f.write("                  {{ {:3}, {:3}, {:>25} /*{}*/ }},  // {}\n".format(midx, cartidx, str(N(v, 18)), v, k))

        src.f.write("      },\n")
        src.f.write("    },\n")
    src.f.write("};\n")


