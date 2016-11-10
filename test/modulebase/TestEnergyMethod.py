from TestFxns import *

tester=Tester("Testing the EnergyMethod module base type")

class FakeEnergyMethod(psr.EnergyMethod):
    def __init__(self,myid):
      """Registers this module with ModuleManager.  Internal use only!!!""" 
      super(FakeEnergyMethod,self).__init__(myid)
    
    def deriv_(self,Order,wfn):
        Mol=wfn.system
        egy=[0.0]
        for Ai in Mol:
            egy[0]+=0.5*(Ai[0]*Ai[0]+Ai[1]*Ai[1]+Ai[2]*Ai[2])
        return wfn,egy

def insert_supermodule():
    cf = ModuleCreationFuncs()
    cf.add_py_creator("FakeEnergyMethod",FakeEnergyMethod)
    return cf

def run(mm,tester):
    om=psr.OptionMap()
    om.add_option("MAX_DERIV",psr.OptionType.Int,False,None,"",0)
    om.add_option("FDIFF_DISPLACEMENT",psr.OptionType.Float,False,None,"",0.005)
    om.add_option("FDIFF_STENCIL_SIZE",psr.OptionType.Int,False,None,"",3)
    minfo=psr.ModuleInfo()
    minfo.name="FakeEnergyMethod"
    minfo.type="c_module"
    minfo.base="EnergyMethod"
    minfo.path="./TestEnergyMethod.so"
    minfo.version="1.0"
    minfo.description="A 3*Natoms H.O."
    minfo.options=om
    mm.load_module_from_minfo(minfo,"Test the H.O.")
    egy_mod=mm.get_module("Test the H.O.",0)
    wfn=psr.Wavefunction()
    H,H1=psr.create_atom([0.0,0.0,0.0],1),psr.create_atom([0.0,0.0,0.89],1)
    MyU=psr.AtomSetUniverse()
    MyU.insert(H);
    MyU.insert(H1);
    wfn.system=psr.System(MyU,True)
    
    #The right answers
    egy=[0.39605]
    grad=[0.0 for i in range(6)]
    grad[5]=0.89
    hess=[0.0 for i in range(36)]
    for i in range(0,36,7):hess[i]=1.0;
    
    deriv=egy_mod.deriv(0,wfn)
    tester.test_value("Energy works",egy,deriv[1])
    
    deriv=egy_mod.deriv(1,wfn)
    tester.test_value("Grad has right dimensions",len(grad),len(deriv[1]))
    for i in range(len(grad)):
        tester.test_value("FDiff grad comp "+str(i),grad[i],deriv[1][i])
    
    deriv=egy_mod.deriv(2,wfn)
    tester.test_value("Hessian has right dimensions",len(hess),len(deriv[1]))
    for i in range(len(hess)):
        tester.test_value("FDiff Hessian comp "+str(i),hess[i],deriv[1][i])
    
with psr.ModuleAdministrator() as mm:
    run(mm,tester)
psr.finalize()
tester.print_results()

