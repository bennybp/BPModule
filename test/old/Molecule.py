#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.system import System, CreateAtom


def Run():
    try:
        out = bp.output.GetGlobalOut()

        tester = bp.testing.Tester("Testing System class")
        tester.PrintHeader()

        atoms = [ CreateAtom(0, [ 0.000000000000,     0.000000000000,     0.000000000000], 6),
                  CreateAtom(1, [ 0.000000000000,     0.000000000000,     2.845112131228], 6),
                  CreateAtom(2, [ 1.899115961744,     0.000000000000,     4.139062527233], 8),
                  CreateAtom(3, [-1.894048308506,     0.000000000000,     3.747688672216], 1),
                  CreateAtom(4, [ 1.942500819960,     0.000000000000,    -0.701145981971], 1),
                  CreateAtom(5, [-1.007295466862,    -1.669971842687,    -0.705916966833], 1),
                  CreateAtom(6, [-1.007295466862,     1.669971842687,    -0.705916966833], 1),

                  # same as above, shifted 2,2,2
                  CreateAtom(7, [ 2.000000000000,  2.000000000000,  2.000000000000 ], 6),
                  CreateAtom(8, [ 2.000000000000,  2.000000000000,  4.845112131228 ], 6),
                  CreateAtom(9, [ 3.899115961744,  2.000000000000,  6.139062527233 ], 8),
                  CreateAtom(10, [ 0.105951691494,  2.000000000000,  5.747688672216 ], 1),
                  CreateAtom(11, [ 3.942500819960,  2.000000000000,  1.298854018029 ], 1),
                  CreateAtom(12, [ 0.992704533138,  0.330028157313,  1.294083033167 ], 1),
                  CreateAtom(13, [ 0.992704533138,  3.669971842687,  1.294083033167 ], 1)
        ]


        molu = bp.system.AtomSetUniverse()
        for a in atoms:
            molu.Insert(a)

        tester.TestValue("System universe size", molu.size(), 14)

        mol = bp.system.System(molu, True)
        tester.TestValue("System size", mol.size(), 14)
        tester.TestValue("System size", len(mol), 14)

        # Does the system contain all the atoms?
        idx = 0
        for a in atoms:
            tester.Test("System has atom {}".format(idx), True, mol.HasAtom, a)
            tester.Test("System has atom {} - via 'in'".format(idx), True, lambda m, el: el in m, mol, a)
            idx += 1

        noatom = CreateAtom(14, [ 0.0, 0.0, 0.0 ], 7)
        tester.Test("System doesn't have atom", False, mol.HasAtom, noatom)
        tester.Test("System doesn't have atom - via 'in'", False, lambda m, el: el in m, mol, noatom)

        # Starting with empty system
        mol = bp.system.System(molu, False)
        tester.TestValue("System size", mol.size(), 0)
        tester.TestValue("System size", len(mol), 0)

        idx = 0
        for a in atoms:
            tester.Test("System has atom {}".format(idx), False, mol.HasAtom, a)
            tester.Test("Inserting atom {}".format(idx), True, InsertAtom, mol, a)
            tester.Test("System has atom {}".format(idx), True, mol.HasAtom, a)
            idx += 1


        tester.Test("Inserting atom not in universe", False, InsertAtom, mol, noatom)

        #PrintMol(mol, out)

        #mol2 = mol.Translate( [ 2.0, 3.0, 4.0 ] )
        #PrintMol(mol2, out)

        #mol2 = mol.Rotate( [ 0.0, 1.0, 0.0,
        #                     0.0, 0.0, 1.0,
        #                     1.0, 0.0, 0.0 ] )
        #PrintMol(mol2, out)

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
