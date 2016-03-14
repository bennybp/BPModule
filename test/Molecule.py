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
    bp.output.GlobalOutput("Number of atoms: {}\n".format(mol.NAtoms()))
    print(mol)
    bp.output.GlobalOutput("Center of Mass: {}\n".format(mol.CenterOfMass().GetCoords()))

def Run():
    try:

        tester = bp.testing.Tester("Testing System class")
        tester.PrintHeader()


        molu = bp.system.AtomSetUniverse()
        molu.append(bp.system.CreateAtom(0, [ 0.000000000000,     0.000000000000,     0.000000000000], 6))
        molu.append(bp.system.CreateAtom(1, [ 0.000000000000,     0.000000000000,     2.845112131228], 6))
        molu.append(bp.system.CreateAtom(2, [ 1.899115961744,     0.000000000000,     4.139062527233], 8))
        molu.append(bp.system.CreateAtom(3, [-1.894048308506,     0.000000000000,     3.747688672216], 1))
        molu.append(bp.system.CreateAtom(4, [ 1.942500819960,     0.000000000000,    -0.701145981971], 1))
        molu.append(bp.system.CreateAtom(5, [-1.007295466862,    -1.669971842687,    -0.705916966833], 1))
        molu.append(bp.system.CreateAtom(6, [-1.007295466862,     1.669971842687,    -0.705916966833], 1))


        # same as above, shifted 2,2,2
        molu.append(bp.system.CreateAtom(7, [ 2.000000000000,  2.000000000000,  2.000000000000 ], 6))
        molu.append(bp.system.CreateAtom(8, [ 2.000000000000,  2.000000000000,  4.845112131228 ], 6))
        molu.append(bp.system.CreateAtom(9, [ 3.899115961744,  2.000000000000,  6.139062527233 ], 8))
        molu.append(bp.system.CreateAtom(10, [ 0.105951691494,  2.000000000000,  5.747688672216 ], 1))
        molu.append(bp.system.CreateAtom(11, [ 3.942500819960,  2.000000000000,  1.298854018029 ], 1))
        molu.append(bp.system.CreateAtom(12, [ 0.992704533138,  0.330028157313,  1.294083033167 ], 1))
        molu.append(bp.system.CreateAtom(13, [ 0.992704533138,  3.669971842687,  1.294083033167 ], 1))


        mol = bp.system.System(molu, True)
        PrintMol(mol)

        mol2 = mol.Translate( [ 2.0, 3.0, 4.0 ] )
        PrintMol(mol2)

        mol2 = mol.Rotate( [ 0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0,
                             1.0, 0.0, 0.0 ] )
        PrintMol(mol2)

        tester.PrintResults() 


    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.GlobalError("\n")
      bp.output.GlobalError(str(e))
      bp.output.GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
