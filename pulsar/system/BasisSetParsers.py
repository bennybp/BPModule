import re
import pulsar as psr

# Break apart a list into blocks, where the original list
# was separated by separator (regex). The first separator is included
# if includesep is True
def block_list(lst, separator, includesep = False):
    curline = 0
    ret = []

    sep = re.compile(separator)

    for i in range(0, len(lst)):
      if sep.match(lst[i]):
          if i != curline:  # If the block isn't empty
              ret.append(lst[curline:i])
          curline = i

          if includesep:
              curline = i+1

    # left over
    if curline != len(lst):
      ret.append(lst[curline:])

    return ret



def read_basis_file(path):
    validcart = ["spherical", "cartesian"]

    basismap = {}

    rawlines = [ l.strip() for l in open(path, 'r').readlines() ]

    # remove comments and 
    lines = [ l for l in rawlines if not l.startswith("!") and l != "" ]

    # Get the type from the first line
    cart = lines[0].lower()
    if not cart in validcart:
        raise PulsarException("Unknown shell type in basis set",
                               "file", path, "type", cart)

    iscart = (cart == "cartesian")
    bstype = psr.ShellType.CartesianGaussian if iscart else psr.ShellType.SphericalGaussian

    atomblocks = block_list(lines[1:], r"\*\*\*\*", True) 

    for atomlines in atomblocks:
        shellvec = []
        element, num = atomlines[0].split()
        shellblocks = block_list(atomlines[1:], r"\D+ *\d+ *\d+(\.\d+)?")

        for shelllines in shellblocks:
            am, nprim, num = shelllines[0].split()
            nprim = int(nprim)

            rawprims = [ l.split() for l in shelllines[1:] ]
            prims = [  [ p[0], p[1:] ] for p in rawprims ]
            if len(prims) != nprim:
                raise PulsarException("Problem parsing basis set: nprim not equal to the actual number of primitives",
                                       "element", element, "file", path, "nprim", nprim, "actual", len(prims))

            # Check to see if all primitives have the same number of
            # general contractions
            for p in prims:
                ngen = len(prims[0][1])
                if len(p[1]) != ngen:
                    raise psr.PulsarException("Ragged number of general contractions",
                                           "element", element, "file", path, "nprim", nprim,
                                           "expected", ngen, "actual", len(p[1]))

            # We have all the information to create a shell now
            amint = psr.string_to_am(am)
            bsi = psr.BasisShellInfo(bstype, amint, nprim, ngen)
            for i in range(0, len(prims)):
                bsi.set_primitive(i, float(prims[i][0]),
                                    [ float(d) for d in prims[i][1] ])

            shellvec.append(bsi)

        element_Z = psr.atomic_z_from_symbol(element)
        basismap[element_Z] = shellvec
                
 
    return basismap

