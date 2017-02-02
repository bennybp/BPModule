import os
import functools
import pulsar as psr

def transform_name(bsname):
    return bsname.replace("*", "s")


def add_shells_to_atom(label, desc, bsmap, atom):
    atom2 = psr.Atom(atom)

    binfo = psr.BasisInfo()
    binfo.description = desc
    binfo.shells = bsmap[atom2.Z]
    

    # Note - remember that the map of basis sets is not opaque
    bs = atom2.basis_sets;    # ^^ bs is a python dict not connected to atom2.basis_sets
    bs[label] = binfo
    atom2.basis_sets = bs

    return atom2


def apply_single_basis(bslabel, bsname, syst):
    # find the file
    # look through all the paths

    bspath = None

    for p in psr.pulsar_paths["basis"]:
        testpath = os.path.join(p, transform_name(bsname)) + ".gbs"
        if os.path.isfile(testpath):
            bspath = testpath
            break
          
    if bspath == None:
        ge = psr.PulsarException("File for basis set does not exist", "bsname", bsname)
        for p in psr.pulsar_paths["basis"]:
            ge.append_info("path", p)
        raise ge

    # read the map
    bsmap = psr.read_basis_file(bspath)

    # Apply to all atoms
    f = functools.partial(add_shells_to_atom, bslabel, bsname, bsmap)
    return syst.transform(f)
 

