#!/usr/bin/env python3
import os
import sys

thispath = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(thispath),"helper"))

from MiscFxns import *
from StandardModules import *

def ApplyBasis(syst,bsname,bslabel="primary"):
    return psr.system.apply_single_basis(bslabel,bsname,syst)

def CompareEgy(EgyIn):
   return EgyIn+228.11521111389305<0.00001

def CompareGrad(GradIn):
   CorrectGrad=[
    -0.006279464066421052, -0.002688502472263158, 0.017376164549999996,
     0.012173863455368422, -0.00017413037057894734, -0.016439835997789472,
     -0.008070814829947367, -0.0008539125617368421, -0.004133039731,
     -0.0007373577561578947, -0.014813456446526319, -0.011724073861368419,
     0.0033722650953157893, 0.003745745449578947, -0.0076003668459473685,
     0.002851177203263159, 0.01279772943342105, 0.01858454261631579,
     0.011151683116631578, 0.01558758901536842, -0.0036400231553157894,
     -0.01853777866094737, -0.012176382615052633, -0.001044487247842105,
     0.004076426441473684, -0.0014246794266842104, 0.008621119674789474]


   AllGood=True
   for i in range(0,len(CorrectGrad)):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing CBS Extrapolations")
        tester.print_header()
       
        LoadDefaultModules(mm)
        mm.DuplicateKey("PSR_MP2","MP2_aTZ")
        mm.change_option("PSR_MP2","BASIS_SET","aug-cc-pVDZ")
        mm.change_option("MP2_aTZ","BASIS_SET","aug-cc-pVTZ")
        mm.change_option("PSR_HELGAKER_CBS","BASIS_CARDINAL_NUMS",[2,3])
        mm.change_option("PSR_MIM","METHODS",["PSR_MP2","MP2_aTZ"])

 
        MyMod=mm.get_module("PSR_HELGAKER_CBS",0)
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
        mol = ApplyBasis(mol,"aug-cc-pvdz","aug-cc-pVDZ")
        mol = ApplyBasis(mol,"aug-cc-pvtz","aug-cc-pVTZ")
        wfn=psr.datastore.Wavefunction()
        wfn.system=mol
        

        NewWfn,Egy=MyMod.Deriv(0,wfn)
        tester.test("Testing Energy via Deriv(0)", True, CompareEgy, Egy[0])
        NewWfn,Egy=MyModenergy(wfn)
        tester.test("Testing Energy via Energy()", True, CompareEgy, Egy)
        NewWfn,Egy=MyMod.Deriv(1,wfn)
        tester.test("Testing Gradient via Deriv(1)", True, CompareGrad, Egy)
        NewWfn,Egy=MyMod.Gradient(wfn)
        tester.test("Testing Energy via Gradient()", True, CompareGrad, Egy)
        
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.finalize()
