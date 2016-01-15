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
    
        MyPoint=bp.math.Point(1.2,1.3,1.4)
        bp.output.Output("%1%\n",str(MyPoint))
    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")

bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()