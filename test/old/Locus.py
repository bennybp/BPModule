import os
import sys
import argparse
import copy
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr

def Run():
    try:
        tester = psr.testing.Tester("Testing construction of Locus Class")
        tester.PrintHeader()
    
        O=psr.system.Atom(8,0.0,0.0,0.0)
        H1=psr.system.Atom(1,0.0,0.0,0.89)
        H2=psr.system.Atom(1,0.0,0.89,0.0)
        Water=psr.system.System
        Water.AddAtom(O)
        Water.AddAtom(H1)
        Water.AddAtom(H2)

        psr.output.GlobalOutput("{}\n".format(str(Water)))

    except Exception as e:
      psr.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")

psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
