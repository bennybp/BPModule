#!/usr/bin/env python3

import os
import sys

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "../", "modules")
sys.path.insert(0, bppath)

from StandardModules import *

def ApplyBasis(syst,bsname,bslabel="primary",bstype=bp.system.ShellType.Gaussian):
    return bp.system.ApplySingleBasis(bstype,bslabel,bsname,syst)

def CompareEgy(EgyIn):
   return EgyIn+224.88878115622086<0.00001

def CompareGrad(GradIn):
   CorrectGrad=[
      -0.0017069898160000001, -0.001626469551000001, 0.050303614141000014, 
       0.015141013799999999, -0.0037465124789999994, -0.02830022157100001,
      -0.02381044127600001, -0.006742533005, -0.025865760775000003,
      -0.013301246809, -0.04562762713499999, -0.012875201989,
       0.017484176375, 0.025399369801, -0.017641048981999998,
       0.00785578526, 0.023461865716000004, 0.01942375991700001,
       0.017700039130999996, 0.040563140105, -0.02590982685799999,
      -0.023815472778, -0.019776749375, 0.007282793088999999,
       0.00445313611, -0.011904484072999997, 0.03358189302100002]

   AllGood=True
   for i in range(0,27):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = bp.testing.Tester("Testing VMFC via MIM")
        tester.PrintHeader()
        
        mm.ChangeOption("BP_VMFC","METHOD","BP_SCF")
        mm.ChangeOption("BP_BOND_FRAG","TRUNCATION_ORDER",2)       
 
        MyMod=mm.GetModule("BP_VMFC",0)
        mol=bp.system.MakeSystem("""
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
        mol = ApplyBasis(mol,"sto-3g")
        wfn=bp.datastore.Wavefunction()
        wfn.system=mol
        MyMod.SetInitialWfn(wfn)
        

        Egy=MyMod.Deriv(0)
        tester.Test("Testing Energy via Deriv(0)", True, CompareEgy, Egy[0])
        Egy=MyMod.Energy()
        tester.Test("Testing Energy via Energy()", True, CompareEgy, Egy)
        Egy=MyMod.Deriv(1)
        tester.Test("Testing Gradient via Deriv(1)", True, CompareGrad, Egy)
        Egy=MyMod.Gradient()
        tester.Test("Testing Energy via Gradient()", True, CompareGrad, Egy)
        
     
    except Exception as e:
      bp.output.Output("Caught exception in main handler\n")
      traceback.print_exc()


with bp.ModuleAdministrator() as mm:
    Run(mm)

bp.Finalize()