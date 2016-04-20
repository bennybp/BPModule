#!/usr/bin/env python3
import os
import sys

thispath = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(thispath),"helper"))

from MiscFxns import *
from StandardModules import *

def CompareEgy(EgyIn):
   return EgyIn+228.99856104845924<0.00001

def CompareGrad(GradIn):
   CorrectGrad=[
     -0.005502867455371351, -0.0027751336381018924, 0.012084950740151623, 
      0.009433403388612161, -0.00046749509597486527, -0.01297325682844054, 
     -0.005722311322994865, -0.00016173311193513505, -0.002000942005888109, 
      0.0006610108084962162, -0.010009097688745408, -0.009957460723114053, 
      0.0016319575067324323, 0.001697567271209459, -0.0056374571379951355, 
      0.0027764939686429737, 0.009729630525249732, 0.015009657098031084, 
      0.00862971968666757, 0.01161260596748973, -0.0011614628661475676, 
     -0.01533113508033946, -0.009051964433770002, -0.0008451939530332433, 
      0.0034237284980951355, -0.0005743798008410808, 0.00548116566911838]
   AllGood=True
   for i in range(0,len(CorrectGrad)):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = psr.testing.Tester(
           "Testing FPA. MP2/a(TQ)Z+[2B-CCSD(T)/aDZ]-[2B-MP2/aDZ]")
        tester.PrintHeader()
       
        LoadDefaultModules(mm)
        mm.DuplicateKey("PSR_MP2","MP2_aTZ")
        mm.DuplicateKey("PSR_MP2","MP2_aQZ")
        mm.DuplicateKey("PSR_MBE","PSR_MBE_MP2")

        aDZ="aug-cc-pvdz"
        aTZ="aug-cc-pvtz"
        aQZ="aug-cc-pvqz"
        mm.ChangeOption("PSR_MBE","METHOD","PSR_CCSD(T)")
        mm.ChangeOption("PSR_MBE_MP2","METHOD","PSR_MP2")
        mm.ChangeOption("PSR_BOND_FRAG","TRUNCATION_ORDER",2)
        mm.ChangeOption("PSR_CCSD(T)","BASIS_SET",aDZ)
        mm.ChangeOption("PSR_MP2","BASIS_SET",aDZ)
        mm.ChangeOption("MP2_aTZ","BASIS_SET",aTZ)
        mm.ChangeOption("MP2_aQZ","BASIS_SET",aQZ)
        

        mm.DuplicateKey("PSR_MIM","FPA_MP2_MIM")
        mm.ChangeOption("FPA_MP2_MIM","METHODS",["MP2_aTZ","MP2_aQZ"])
        mm.ChangeOption("PSR_HELGAKER_CBS","BASIS_CARDINAL_NUMS",[3,4])
        mm.ChangeOption("PSR_HELGAKER_CBS","MIM_KEY","FPA_MP2_MIM")
        mm.ChangeOption("PSR_MIM","METHODS",["PSR_HELGAKER_CBS",
                                            "PSR_MBE",
                                            "PSR_MBE_MP2"
                                            ])
        mm.ChangeOption("PSR_MIM","WEIGHTS",[1.0,1.0,-1.0])
        MyMod=mm.GetModule("PSR_MIM",0)
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
        mol = ApplyBasis(mol,aDZ,aDZ)
        mol = ApplyBasis(mol,aTZ,aTZ)
        mol = ApplyBasis(mol,aQZ,aQZ)
        wfn=psr.datastore.Wavefunction()
        wfn.system=mol
        MyMod.SetInitialWfn(wfn)
        

        Egy=MyMod.Deriv(1)
        tester.Test("Testing Energy via Deriv(0)", True, CompareEgy, Egy[0])
        Egy=MyMod.Energy()
        tester.Test("Testing Energy via Energy()", True, CompareEgy, Egy)
        Egy=MyMod.Deriv(1)
        tester.Test("Testing Gradient via Deriv(1)", True, CompareGrad, Egy)
        Egy=MyMod.Gradient()
        tester.Test("Testing Energy via Gradient()", True, CompareGrad, Egy)
        
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()

