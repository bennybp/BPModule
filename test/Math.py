#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")

import bpmodule as bp


def Run():
    try:

        tester = bp.testing.Tester("Testing construction of OptionMap objects")
        tester.PrintHeader()

        for i in range(1, 10):
            bp.output.Output("I = %1%\n", i)
            bp.output.Output("%1%   %2%\n", str(type(bp.math.Factorial(i))), bp.math.Factorial(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.FactorialF(i))), bp.math.FactorialF(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.FactorialD(i))), bp.math.FactorialD(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.DoubleFactorial(i))), bp.math.DoubleFactorial(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.DoubleFactorialF(i))), bp.math.DoubleFactorialF(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.DoubleFactorialD(i))), bp.math.DoubleFactorialD(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.Double2nm1Factorial(i))), bp.math.Double2nm1Factorial(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.Double2nm1FactorialF(i))), bp.math.Double2nm1FactorialF(i))
            bp.output.Output("%1%   %2%\n", str(type(bp.math.Double2nm1FactorialD(i))), bp.math.Double2nm1FactorialD(i))
            bp.output.Output("\n")



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
