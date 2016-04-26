#!/usr/bin/env python3

import os
import sys

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

#from StandardModules import *
from helper.Molecules import *

def TestPG(tester,Sym,PG):
    g=lambda x: x.SSymbol==Sym
    tester.Test(Sym,True,g,PG)

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing Symmetry")
        tester.PrintHeader()

        Syms=["Cs","Ci",
              "C2","C3",#C4,C5,C6,
              "C2v","C3v","C4v","C5v",#C6v,
              "C2h","C3h",#C4h,C5h,C6h
              "D2",#D3,D4,D5,D6
              "D2h","D3h","D4h","D5h","D6h",
              "D2d","D3d","D4d","D5d",#D6d,
              "S4",#S6,S8,S10,
              #T,Td,Th,O,Oh,I,Ih,
              "Coov","Dooh","Kh"
        ]
        Mols=[CH2BrCl,PeroxideDimer,
              Peroxide,NO3H3,
              Water,Ammonia,BrF5,Corannulene,
              Diazene,BoricAcid,
              Twistane,
              Napthalene,EthaneEclip,PtCl4,C5H5Anion,Benzene,
              Allene,EthaneStag,S8,FerroceneStag,
              C8F4H4,
              C2HCl,C2H2,He
        ]
       
        #LoadDefaultModules(mm)
        Symmer=psr.system.Symmetrizer()
        
        for i in range(0,len(Syms)):
            TestPG(tester,Syms[i],Symmer.GetSymmetry(Mols[i]))
        
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()

