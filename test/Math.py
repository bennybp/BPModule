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

def Run():
    try:

        tester = bp.testing.Tester("Testing Math Library")
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

        MyUniverse=bp.math.Universe()
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        for i in list(Alphabet):
           MyUniverse.append(i)
        MySets=[]
        MySets.append(bp.math.MathSet(MyUniverse))
        
        #Set 0 is just "a"
        MySets[0].append("a")      


        #Set 1 is the rest of the alphabet (test complement)
        MySets.append(MySets[0].Complement())

        #Set 2 is "abr" (tests copy)
        MySets.append(bp.math.MathSet(MySets[0]))
        MySets[2].append("b")
        MySets[2].append("r")
        
        #Set 3 is "br" (test intersection)
        MySets.append(MySets[2].__div__(MySets[1]))
            
        #Set 4 is "a" (test set-difference)
        MySets.append(MySets[2]-MySets[1])
       
        #Set 5 is "abr" (test union)
        MySets.append(MySets[3]+MySets[2])

        #Set 6 is "A" (transform MySets[0])
        MySets.append(MySets[0].Transform(lambda c: c.upper()))

        #Set 7 should be the same as Set 1 (only elements of Set 0 are transformed)
        MySets.append(MySets[6].Complement())
        
        #Finally test that MyUniverse is really a shared_ptr, is being passed
        #that way, and the reference count is not completely messed up
        del MyUniverse
        SetComp=lambda set1,set2 : (str(set1).replace(" ","")==set2)
        tester.Test("Complement",True,SetComp,MySets[1],Alphabet[1:26])  
        tester.Test("Intersection",True,SetComp,MySets[3],"br") 
        tester.Test("Set-difference",True,SetComp,MySets[4],"a")
        tester.Test("Union",True,SetComp,MySets[5],"abr")         
        tester.Test("Transform",True,SetComp,MySets[6],"A")
        tester.Test("Complement",True,SetComp,MySets[7],Alphabet[1:26])

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
