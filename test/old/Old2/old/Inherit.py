#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run(mm):
    try:
      out = psr.output.GetGlobalOut()

      # Load the python modules
      #             supermodule      module name      key
      mm.LoadModule("TestModules",   "TestModule1",   "TESTMOD1")
      mm.LoadModule("TestModules",   "TestPyModule1", "TESTPYMOD1")

      mm.ChangeOption("TESTMOD1", "double_opt_def", 1.111)
      mm.ChangeOption("TESTMOD1", "int_opt_def", 55 )
      mm.Print(out)
      mm.SanityCheck()


      # Create the molecule/basis set
      mol = psr.system.System()
      mol.AddAtom(1, [0.0, 0.0, 0.0] )
      mol.AddAtom(1, [1.0, 0.0, 0.0] )
      mol.AddAtom(8, [0.0, 1.0, 0.0] )


      bs = psr.system.CreateSimpleBasisSet("/home/ben/programming/BPModule/install/basis/sto-3g.gbs", mol);

      psr.output.GlobalOutput("Number of shells: {}\n".format(bs.NShell()))
      for i in range(0, bs.NShell()):
         gs = bs.Shell(i)
         print("ID={}  Cart={}   NPrim={}   {}".format(gs.ID(), gs.IsCartesian(), gs.NPrim(), gs.Coordinates()))
         for j in range(0, gs.NPrim()):
           print("      Prim {:<4} :  {:<10}    {:<10}".format(j, gs.Alpha(j), gs.Coef(j)))


      b1 = mm.GetModule("TESTMOD1", 0)
      b1.Wfn().basis.Set(bs)
      b1.Wfn().molecule.Set(mol)

      b2 = mm.GetModule("TESTPYMOD1", b1.ID())

      b1.RunTest()
      psr.output.GlobalOutput("\n")
      b2.RunTest()
      psr.output.GlobalOutput("\n")
    
      psr.output.GlobalOutput("\nDone testing\n")


    except Exception as e:
      psr.output.GlobalOutput("Caught exception in main handler\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")



psr.Init(sys.argv, out = "stdout", color = True, debug = True)

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
