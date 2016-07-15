#!/usr/bin/env python3
import os
import sys

thispath = os.path.dirname(os.path.realpath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(thispath),"helper"))

from MiscFxns import *
from StandardModules import *
import pulsar_psi4



def ApplyBasis(syst,bsname,bslabel="primary"):
    return psr.system.apply_single_basis(bslabel,bsname,syst)

def CompareEgy(EgyIn):
   return abs(EgyIn+224.89287653924677)<0.00001

def CompareGrad(GradIn):
   CorrectGrad=[
    -0.000988976949000001, 0.0004443157829999993, 0.05238342271999999, 
     0.018237358511, -0.002547005771, -0.030731839919000005, 
    -0.02344281975, -0.0062568701740000005, -0.025360880303, 
    -0.015409293889000001, -0.047382578540999996, -0.012807191666999996, 
     0.016869055227000003, 0.024963490952999996, -0.017442968207000004, 
     0.007207092293000001, 0.025306999363999997, 0.023850402741000004, 
     0.019786523729999998, 0.04038960502300001, -0.028509120090000006, 
    -0.026869925129, -0.022975320699000004, 0.005627050168, 
     0.004610985953999999, -0.011942635934, 0.032991124551000006]

   AllGood=True
   for i in range(0,len(CorrectGrad)):
        AllGood=AllGood and CorrectGrad[i]-GradIn[i]<0.00001
   return AllGood

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing Boys and Bernardi CP")
        tester.print_header()
        pulsar_psi4.pulsar_psi4_setup(mm)
        LoadDefaultModules(mm)
        mm.change_option("PSI4_SCF","BASIS_SET","sto-3g")
        mm.change_option("PSR_CP","METHOD","PSI4_SCF")
        mm.change_option("PSR_MBE","METHOD","PSI4_SCF")

        mol=psr.system.make_system("""
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

        MyMod=mm.get_module("PSR_CP",0)

        NewWfn,Egy=MyMod.deriv(0,wfn)
        print(Egy)
        tester.test("Testing CP Energy via Deriv(0)", True, CompareEgy, Egy[0])
        return()
        NewWfn,Egy=MyMod.energy(wfn)
        tester.test("Testing CP Energy via Energy()", True, CompareEgy, Egy)
        NewWfn,Egy=MyMod.deriv(1,wfn)
        tester.test("Testing CP Gradient via Deriv(1)", True, CompareGrad, Egy)
        NewWfn,Egy=MyMod.gradient(wfn)
        tester.test("Testing CP Gradient via Gradient()", True, CompareGrad, Egy)
        
        tester.print_results()
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.finalize()
