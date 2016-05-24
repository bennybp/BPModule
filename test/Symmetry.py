#!/usr/bin/env python3

import os
import sys
import copy

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

#from StandardModules import *
from helper.Molecules import *

def RightEs(PG,Elems):
    Es=copy.deepcopy(Elems[PG.SSymbol])
    for i in PG.Elems:
        if Es.count(i.SSymbol)>0:
            Es.remove(i.SSymbol)
        else:
            return False
    return True if len(Es)==0 else False

def RightOrder(PG,Elems):
    if PG.Finite():
        return PG.Order()==len(Elems[PG.SSymbol])
    return True

def TestPG(tester,IPG,PG,Elems):
    """
    This function first tests if our idealized point group, IPG,
    has the correct elements by comparing to Elems[IPG.SSymbol].
    
    Next, it determines if our non idealized point group has the
    correct symbol.
    
    Finally, we check if the non idealized point group has the righ
    elements
    """
    SS=IPG.SSymbol
    RightEs(IPG,Elems)
    Name="Idealized "+SS
    tester.Test(Name+" has correct elements",True,RightEs,IPG,Elems)
    tester.Test(Name+" has correct order",True,RightOrder,IPG,Elems)
    Name="Computed "+SS
    tester.Test(Name+" has correct elements",True,RightEs,PG,Elems)
    tester.Test(Name+" has correct order",True,RightOrder,PG,Elems)
    

def Run(mm):
    try:
        tester = psr.testing.Tester("Testing Symmetry")
        tester.PrintHeader()
        
        Cs=psr.system.Cs()
        Ci=psr.system.Ci()
        C2=psr.system.Cn(2)
        C3=psr.system.Cn(3)
        C2v=psr.system.Cnv(2)
        C3v=psr.system.Cnv(3)
        C4v=psr.system.Cnv(4)
        C5v=psr.system.Cnv(5)
        C2h=psr.system.Cnh(2)
        C3h=psr.system.Cnh(3)
        D2=psr.system.Dn(2)
        D2h=psr.system.Dnh(2)
        D3h=psr.system.Dnh(3)
        D4h=psr.system.Dnh(4)
        D5h=psr.system.Dnh(5)
        D6h=psr.system.Dnh(6)
        D2d=psr.system.Dnd(2)
        D3d=psr.system.Dnd(3)
        D4d=psr.system.Dnd(4)
        D5d=psr.system.Dnd(5)
        S4=psr.system.Sn(4)
        Coov=psr.system.Coov()
        Dooh=psr.system.Dooh()
        Kh=psr.system.Kh()
        Elems={"Cs":["E","s"],
               "Ci":["E","i"],
               "C1":["E"],
               "C2":["E","C_2"],
               "C3":["E","C_3","C_3^2"],
               "C2v":["E","C_2","s","s"],
               "C3v":["E","C_3","C_3^2","s","s","s"],
               "C4v":["E","C_4","C_2","C_4^3","s","s","s","s"],
               "C5v":["E","C_5","C_5^2","C_5^3","C_5^4","s","s","s","s","s"],
               "C2h":["E","C_2","i","s"],
               "C3h":["E","C_3","C_3^2","s","S_3","S_3^5"],
               "D2":["E","C_2","C_2","C_2"],
               "D2":["E","C_2","C_2","C_2"],
               "D2h":["E","C_2","C_2","C_2","s","s","s","i"],
               "D3h":["E","C_3","C_3^2","s","S_3","S_3^5","s","s","s",
                      "C_2","C_2","C_2"],
               "D4h":["E","C_4","C_4^3","C_2","C_2","C_2","C_2","C_2","i",
                      "S_4","S_4^3","s","s","s","s","s"],
               "D5h":["E","C_5","C_5^4","C_5^2","C_5^3","C_2","C_2","C_2",
                      "C_2","C_2","S_5","S_5^9","S_5^3","S_5^7",
                       "s","s","s","s","s","s"],
               "D6h":["E","C_6","C_6^5","C_3","C_3^2",
                      "C_2","C_2","C_2","C_2","C_2","C_2","C_2",
                      "i","S_3","S_3^5","S_6","S_6^5","s","s","s","s","s",
                      "s","s"],
               "D2d":["E","S_4","S_4^3","C_2","C_2","C_2","s","s"],
               "D3d":["E","C_3","C_3^2","C_2","C_2","C_2","i","S_6","S_6^5",
                      "s","s","s"],
               "D4d":["E","S_8","S_8^3","S_8^5","S_8^7","C_4","C_4^3","C_2",
                      "C_2","C_2","C_2","C_2","s","s","s","s"],
               "D5d":["E","C_5","C_5^2","C_5^3","C_5^4",
                      "C_2","C_2","C_2","C_2","C_2","i",
                      "S_10","S_10^9","S_10^3","S_10^7","s","s","s","s","s"],
               "S4":["E","S_4","S_4^3","C_2"],
               "Coov":["E","C_oo"],
               "Dooh":["E","C_oo","i"],
               "Kh":["E"]                    
        }

        Syms=[Cs,Ci,C2,C3,#C4,C5,C6,
              C2v,C3v,C4v,C5v,#C6v,
              C2h,C3h,#C4h,C5h,C6h
              D2,#D3,D4,D5,D6
              D2h,D3h,D4h,D5h,D6h,
              D2d,D3d,D4d,D5d,#D6d,
              S4,#S6,S8,S10,
              Coov,Dooh,Kh,
              "Td","Oh","Ih",#T,Th,O,I,
        ]
        Mols=[CH2BrCl,PeroxideDimer,Peroxide,NO3H3,
              Water,Ammonia,BrF5,Corannulene,
              Diazene,BoricAcid,
              Twistane,
              Napthalene,EthaneEclip,PtCl4,C5H5Anion,Benzene,
              Allene,EthaneStag,S8,FerroceneStag,
              C8F4H4,
              C2HCl,C2H2,He,
              Methane,SF6,C60,
        ]
       
        #LoadDefaultModules(mm)
        Symmer=psr.system.Symmetrizer()
        f=open("Test.xyz","w")
        f.write(str(BzCrystal.Size())+"\n\n")
        for i in BzCrystal:
            f.write(i.GetSymbol()+" "+str(i[0]*0.529177)+" "+str(i[1]*0.529177)+" "+str(i[2]*0.529177)+"\n")
        f.close()
        Symmer.GetSymmetry(BzCrystal)
        exit()
        for i in range(0,24):
            TestPG(tester,Syms[i],Symmer.GetSymmetry(Mols[i]),Elems)
        tester.PrintResults()
        
     
    except Exception as e:
      psr.output.Output("Caught exception in main handler\n")
      traceback.print_exc()

with psr.ModuleAdministrator() as mm:
    Run(mm)

psr.Finalize()

