#!/usr/bin/env python3

import sys
import re
import bp_common 


if len(sys.argv) != 3:
   print("Usage: generate_atommass.py outbase ciaawfile")
   quit(1)


outbase = sys.argv[1]


atomre = re.compile(r'^([0-9]+) +([A-Z][a-z]*) +([a-zA-Z]+) +([0-9]+)\*? +([0-9\., \[\]]*) *.*$') 
isore = re.compile(r'^([0-9]+)\*? +([0-9].*).*$')


def FixNumber(n):
  s = n.replace(' ', '')
  s = re.sub(r'\(.*\)', r'', s)
  m = re.match(r'\[(.*),(.*)\]', s)

  if m:
    s = str(float(m.group(1)) / float(m.group(2)))
  
  return float(s)


def ReadCIAAWAbundance(path):
    filelines = [ x.strip() for x in open(path).readlines() ]

    atoms = {} 
    curatom = None


    for line in filelines:
        m = atomre.match(line)
        if m:
            if curatom:
                atoms[curatom['znum']] = curatom

            curatom = { 'znum' : int(m.group(1)),
                        'symbol' : m.group(2),
                        'name' : m.group(3),
                        'isos' : [ ( int(m.group(4)), FixNumber(m.group(5)) ) ] 
                      }

        m = isore.match(line)
        if m:
            curatom['isos'].append( ( int(m.group(1)), FixNumber(m.group(2))) )

    # Append the last one
    if curatom:
      atoms[curatom['znum']] = curatom

    return atoms


atoms = ReadCIAAWAbundance(sys.argv[2])



# Print out what we found
for i in range(0, 120):
  if i in atoms:
    atom = atoms[i]
    print('{:<5} {:<5} {:<25}'.format(atom['znum'], atom['symbol'], atom['name']))

    for iso in atom['isos']:
      print('        {:<5} {:<}'.format(iso[0], iso[1]))
 
    print() 





with bp_common.HeaderSourceFiles(outbase, "LUT for Isotope Abundance", 
                                 ["math", "lut"],
                                 createheader = False,
                                 hppincludes = [],
                                 cppincludes = ["<map>"]) as src:

    # Isotopic composition
    src.f.write("std::map<int, std::map<int, double>> atom_abund_map_{\n")
    for i in range(0, 200):
      if i in atoms:
        atom = atoms[i]
    
        src.f.write('    {{  {:5<} ,  {{    // {}\n'.format(i, atom['name']))
        for iso in atom['isos']:
             src.f.write('                {{ {:5<} , {:<} }},\n'.format(iso[0], iso[1]))
        src.f.write('            },\n')
        src.f.write('    },\n')
    src.f.write('};\n\n\n')


    # Most common isotope
    src.f.write("std::map<int, int> atom_commoniso_map_{\n")
    for i in range(0, 200):
      if i in atoms:
        atom = atoms[i]
    
        maxabiso = (0, 0)
        for iso in atom['isos']:
            if iso[1] > maxabiso[1]:
                maxabiso = iso
        src.f.write('    {{  {:5<} , {:5<} }},    // {}\n'.format(i, maxabiso[0], atom['name']))
    src.f.write('};\n\n\n')
    




