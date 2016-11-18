#!/usr/bin/env python3


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

        tester = psr.testing.Tester("Testing Math Library")
        tester.PrintHeader()

        for i in range(1, 10):
            psr.output.GlobalOutput("I = {}\n".format(i))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.Factorial(i))), psr.math.Factorial(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.FactorialF(i))), psr.math.FactorialF(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.FactorialD(i))), psr.math.FactorialD(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.DoubleFactorial(i))), psr.math.DoubleFactorial(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.DoubleFactorialF(i))), psr.math.DoubleFactorialF(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.DoubleFactorialD(i))), psr.math.DoubleFactorialD(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.Double2nm1Factorial(i))), psr.math.Double2nm1Factorial(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.Double2nm1FactorialF(i))), psr.math.Double2nm1FactorialF(i)))
            psr.output.GlobalOutput("{}   {}\n".format(str(type(psr.math.Double2nm1FactorialD(i))), psr.math.Double2nm1FactorialD(i)))
            psr.output.GlobalOutput("\n")

        MyUniverse=psr.math.StringSetUniverse()
        Alphabet="abcdefghijklmnopqrstuvwxyz"
        for i in list(Alphabet):
           MyUniverse.Insert(i)
        MySets=[]
        MySets.append(psr.math.StringSet(MyUniverse, False))
        
        #Set 0 is just "a"
        MySets[0].Insert("a")      


        #Set 1 is the rest of the alphabet (test complement)
        MySets.append(MySets[0].Complement())

        #Set 2 is "abr" (tests copy)
        MySets.append(psr.math.StringSet(MySets[0]))
        MySets[2].Insert("b")
        MySets[2].Insert("r")
        
        #Set 3 is "br" (test intersection)
        MySets.append(MySets[2] / MySets[1])
            
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
      psr.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
