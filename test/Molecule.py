#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def PrintMol(mol):
    bp.output.Output("\nNumber of atoms: {}\n".format(mol.NAtoms()))
    print(mol)

    bp.output.Output("Center of Mass: %1%\n", mol.CenterOfMass().GetCoords())

def Run():
    try:

        tester = bp.testing.Tester("Testing Molecule class")
        tester.PrintHeader()


        molu = bp.system.AtomSetUniverse()
        molu.append(bp.system.CreateAtom([ 0.000000000000,     0.000000000000,     0.000000000000], 6, "frag1"))
        molu.append(bp.system.CreateAtom([ 0.000000000000,     0.000000000000,     2.845112131228], 6, "frag1"))
        molu.append(bp.system.CreateAtom([ 1.899115961744,     0.000000000000,     4.139062527233], 8, "frag1"))
        molu.append(bp.system.CreateAtom([-1.894048308506,     0.000000000000,     3.747688672216], 1, "frag1"))
        molu.append(bp.system.CreateAtom([ 1.942500819960,     0.000000000000,    -0.701145981971], 1, "frag1"))
        molu.append(bp.system.CreateAtom([-1.007295466862,    -1.669971842687,    -0.705916966833], 1, "frag1"))
        molu.append(bp.system.CreateAtom([-1.007295466862,     1.669971842687,    -0.705916966833], 1, "frag1"))


        # same as above, shifted 2,2,2
        molu.append(bp.system.CreateAtom([ 2.000000000000,  2.000000000000,  2.000000000000 ], 6, "frag2"))
        molu.append(bp.system.CreateAtom([ 2.000000000000,  2.000000000000,  4.845112131228 ], 6, "frag2"))
        molu.append(bp.system.CreateAtom([ 3.899115961744,  2.000000000000,  6.139062527233 ], 8, "frag2"))
        molu.append(bp.system.CreateAtom([ 0.105951691494,  2.000000000000,  5.747688672216 ], 1, "frag2"))
        molu.append(bp.system.CreateAtom([ 3.942500819960,  2.000000000000,  1.298854018029 ], 1, "frag2"))
        molu.append(bp.system.CreateAtom([ 0.992704533138,  0.330028157313,  1.294083033167 ], 1, "frag2"))
        molu.append(bp.system.CreateAtom([ 0.992704533138,  3.669971842687,  1.294083033167 ], 1, "frag2"))


        mol = bp.system.Molecule(molu, True)
        PrintMol(mol)

        mol2 = mol.Translate( [ 2.0, 3.0, 4.0 ] )
        PrintMol(mol2)

        mol2 = mol.Rotate( [ 0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0,
                             1.0, 0.0, 0.0 ] )
        PrintMol(mol2)

        tester.PrintResults() 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
