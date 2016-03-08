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

def Run():
    try:
        tester = bp.testing.Tester("Testing construction of Locus Class")
        tester.PrintHeader()
    
        O=bp.system.Atom(8,0.0,0.0,0.0)
        H1=bp.system.Atom(1,0.0,0.0,0.89)
        H2=bp.system.Atom(1,0.0,0.89,0.0)
        Water=bp.system.System
        Water.AddAtom(O)
        Water.AddAtom(H1)
        Water.AddAtom(H2)

        bp.output.Output("%1%\n",str(Water))

    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")

bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()