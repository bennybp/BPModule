#!/usr/bin/env python3
import os
import sys

thispath = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(thispath),"helper"))

from MiscFxns import *
from StandardModules import *

#Correct energies
UnCPEgy=224.90851104986243
CPEgy=224.89287653924677
VMFCEgy=224.88878115622086

#Correct gradients
UnCPGrad=[0.0056338166169999995, 0.0036814809890000004, 0.052711233995, 
          0.019826950933, -0.0014316311719999998, -0.031158701642999997,
          -0.024939313557, -0.0048274306840000005, -0.024943585464,
          -0.018262424115999997, -0.047669106939, -0.006046442297,
           0.015971721606, 0.024963209265, -0.019093643937999998, 
           0.00619578613, 0.025219535787000005, 0.027208442172, 
           0.017581587257, 0.036519045374999996, -0.035085608934999984,
          -0.028617330772000002, -0.023986983217, 0.003537758562,
           0.006609205900000001, -0.012468119404999999, 0.03287054754200001]
CPGrad=[-0.000988976949000001, 0.0004443157829999993, 0.05238342271999999, 
         0.018237358511, -0.002547005771, -0.030731839919000005, 
        -0.02344281975, -0.0062568701740000005, -0.025360880303, 
        -0.015409293889000001, -0.047382578540999996, -0.012807191666999996, 
         0.016869055227000003, 0.024963490952999996, -0.017442968207000004, 
         0.007207092293000001, 0.025306999363999997, 0.023850402741000004, 
         0.019786523729999998, 0.04038960502300001, -0.028509120090000006, 
        -0.026869925129, -0.022975320699000004, 0.005627050168, 
         0.004610985953999999, -0.011942635934, 0.032991124551000006]
VMFCGrad=[-0.0017069898160000001, -0.001626469551000001, 0.050303614141000014, 
           0.015141013799999999, -0.0037465124789999994, -0.02830022157100001,
          -0.02381044127600001, -0.006742533005, -0.025865760775000003,
          -0.013301246809, -0.04562762713499999, -0.012875201989,
           0.017484176375, 0.025399369801, -0.017641048981999998,
           0.00785578526, 0.023461865716000004, 0.01942375991700001,
           0.017700039130999996, 0.040563140105, -0.02590982685799999,
          -0.023815472778, -0.019776749375, 0.007282793088999999,
           0.00445313611, -0.011904484072999997, 0.03358189302100002]

def ApplyBasis(syst,bsname,bslabel="primary"):
    return psr.system.ApplySingleBasis(bslabel,bsname,syst)

def CompareEgy(EgyIn,BenchMarkEgy):
   return abs(EgyIn+BenchMarkEgy)<0.00001

def CompareGrad(GradIn,CorrectGrad):
   AllGood=True
   for i in range(0,27):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing Various Flavors of MBE")
        tester.PrintHeader()

        LoadDefaultModules(mm)

        mm.ChangeOption("PSR_BOND_FRAG","TRUNCATION_ORDER",2)       
        mm.ChangeOption("PSR_SCF","BASIS_SET","sto-3g")
        
        mm.ChangeOption("PSR_MBE","METHOD","PSR_SCF")
        mm.ChangeOption("PSR_MBE","FRAGMENTIZER","PSR_BOND_FRAG")       
        mm.DuplicateKey("PSR_MBE","PSR_VMFC")
        mm.ChangeOption("PSR_VMFC","METHOD","PSR_SCF")
        mm.ChangeOption("PSR_VMFC","FRAGMENTIZER","PSR_VMFC_FRAG")

        
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
        
        
        MyMod=mm.GetModule("PSR_MBE",0)
        NewWfn,Egy=MyMod.Deriv(0,wfn)
        tester.Test("Testing MBE(2) Energy via Deriv(0)",True,CompareEgy,Egy[0],UnCPEgy)
        NewWfn,Egy=MyMod.Energy(wfn)
        tester.Test("Testing MBE(2) Energy via Energy()",True,CompareEgy,Egy,UnCPEgy)
        NewWfn,Grad=MyMod.Deriv(1,wfn)
        tester.Test("Testing MBE(2) Gradient via Deriv(1)",True, CompareGrad,Grad,UnCPGrad)
        NewWfn,Grad=MyMod.Gradient(wfn)
        tester.Test("Testing MBE(2) Gradient via Gradient()",True,CompareGrad,Grad,UnCPGrad)


        MyMod=mm.GetModule("PSR_VMFC",0)
        NewWfn,Egy=MyMod.Deriv(0,wfn)
        tester.Test("Testing VMFC(2) Energy via Deriv(0)",True,CompareEgy,Egy[0],VMFCEgy)
        NewWfn,Egy=MyMod.Energy(wfn)
        tester.Test("Testing VMFC(2) Energy via Energy()",True,CompareEgy,Egy,VMFCEgy)
        NewWfn,Grad=MyMod.Deriv(1,wfn)
        tester.Test("Testing VMFC(2) Gradient via Deriv(1)",True, CompareGrad,Grad,VMFCGrad)
        NewWfn,Grad=MyMod.Gradient(wfn)
        tester.Test("Testing VMFC(2) Gradient via Gradient()",True,CompareGrad,Grad,VMFCGrad)       

        tester.PrintResults()
        
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()
