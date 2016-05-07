#!/usr/bin/env python3
import os
import sys

thispath = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(thispath),"helper"))

from MiscFxns import *
from StandardModules import *

def ApplyBasis(syst,bsname,bslabel="primary"):
    return psr.system.ApplySingleBasis(bslabel,bsname,syst)

def CompareEgy(EgyIn):
   return EgyIn+224.90851104986243<0.00001

def CompareGrad(GradIn):
   CorrectGrad=[0.0056338166169999995, 0.0036814809890000004, 0.052711233995, 
                0.019826950933, -0.0014316311719999998, -0.031158701642999997,
                -0.024939313557, -0.0048274306840000005, -0.024943585464,
                -0.018262424115999997, -0.047669106939, -0.006046442297,
                 0.015971721606, 0.024963209265, -0.019093643937999998, 
                 0.00619578613, 0.025219535787000005, 0.027208442172, 
                 0.017581587257, 0.036519045374999996, -0.035085608934999984,
                -0.028617330772000002, -0.023986983217, 0.003537758562,
               0.006609205900000001, -0.012468119404999999, 0.03287054754200001]
   AllGood=True
   for i in range(0,27):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing MBE via MIM")
        tester.PrintHeader()

        LoadDefaultModules(mm)
        
        mm.ChangeOption("PSR_MBE","METHOD","PSR_SCF")
        mm.ChangeOption("PSR_MIM","FRAGMENTIZER","PSR_BOND_FRAG")
        mm.ChangeOption("PSR_BOND_FRAG","TRUNCATION_ORDER",2)       
        mm.ChangeOption("PSR_SCF","BASIS_SET","sto-3g")
        MyMod=mm.GetModule("PSR_MBE",0)
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
        wfn.SetSystem(mol)
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
