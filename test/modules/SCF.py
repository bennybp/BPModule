#!/usr/bin/env python3
import os
import sys

thispath = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(thispath),"helper"))

from MiscFxns import *
from StandardModules import *

def CompareEgy(EgyIn):
   return EgyIn+224.912529687124<0.00001

def CompareGrad(GradIn):
   CorrectGrad=[
      0.00631057813355, 0.00571458363554, 0.05476152065996, 
      0.02287072160272, -0.0002840915734, -0.03359062789176, 
     -0.02457654725095, -0.00435313214139, -0.02443656592336, 
     -0.02033326759132, -0.04939904659428, -0.00601012407546, 
      0.01536321804528, 0.02452313009004, -0.01889869345071, 
      0.0056070168479, 0.02707750704665, 0.03157680066598, 
      0.01965867456494, 0.03636269982351, -0.03762798149958, 
     -0.03166475907529, -0.02714461080685, 0.00193798500615, 
      0.00676436472219, -0.01249703947853, 0.03228768650336]
   AllGood=True
   for i in range(0,27):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing SCF")
        tester.PrintHeader()

        LoadDefaultModules(mm)
        
        mm.ChangeOption("PSR_SCF","BASIS_SET","sto-3g")
        MyMod=mm.GetModule("PSR_SCF",0)
        mol=psr.system.MakeSystem("""
        0 1
        O    1.2361419   1.0137761  -0.0612424
        H    0.5104418   0.8944555   0.5514190
        H    1.9926927   1.1973129   0.4956931
        O   -0.9957202   0.0160415   1.2422556
        H   -1.4542703  -0.5669741   1.8472817
        H   -0.9377950  -0.4817912   0.4267562
        O   -0.2432343  -1.0198566  -1.1953808
        H    0.4367536  -0.3759433  -0.9973297
        H   -0.5031835  -0.8251492  -2.0957959
        """)
        mol = ApplyBasis(mol,"sto-3g","sto-3g")
        wfn=psr.datastore.Wavefunction()
        wfn.system=mol
        MyMod.SetInitialWfn(wfn)
        

        Egy=MyMod.Deriv(0)
        tester.Test("Testing Energy via Deriv(0)", True, CompareEgy, Egy[0])
        Egy=MyMod.Energy()
        tester.Test("Testing Energy via Energy()", True, CompareEgy, Egy)
        Grad=MyMod.Deriv(1)
        tester.Test("Testing Gradient via Deriv(1)", True, CompareGrad, Grad)
        Grad=MyMod.Gradient()
        tester.Test("Testing Gradient via Gradient()", True, CompareGrad, Grad)

        tester.PrintResults()
        
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
