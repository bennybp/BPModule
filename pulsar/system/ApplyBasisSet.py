import os
import functools
from . import BasisSetParsers
import pulsar
from pulsar.exception import GeneralException


def TransformName(bsname):
    return bsname.replace("*", "s")


def AddShellsToAtom(label, desc, bsmap, atom):
    atom2 = pulsar.system.Atom(atom)
    atom2.SetShells(label, desc, bsmap[atom.GetZ()])
    return atom2


def ApplySingleBasis(bslabel, bsname, syst):
    # find the file
    # look through all the paths

    bspath = None

    for p in pulsar.pulsar_paths["basis"]:
        testpath = os.path.join(p, TransformName(bsname)) + ".gbs"
        if os.path.isfile(testpath):
            bspath = testpath
            break
          
    if bspath == None:
        ge = GeneralException("File for basis set does not exist", "bsname", bsname)
        for p in pulsar.pulsar_paths["basis"]:
            ge.AppendInfo("path", p)
        raise ge

    # read the map
    bsmap = BasisSetParsers.ReadBasisFile(bspath)

    # Apply to all atoms
    f = functools.partial(AddShellsToAtom, bslabel, bsname, bsmap)
    return syst.Transform(f)
 

