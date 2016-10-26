from TestFxns import *

tester = Tester("Testing the BasisSet and BasisSetShell")
cGTO = psr.system.ShellType.CartesianGaussian
sGTO = psr.system.ShellType.SphericalGaussian
alpha=[3.42525091, 0.62391373, 0.16885540]
c=[0.15432897, 0.53532814, 0.44463454]

FakeD=psr.system.BasisShellInfo(cGTO,2,3,1,alpha,c)
FakeD2=psr.system.BasisShellInfo(sGTO,2,3,1,alpha,c)
carts=[0.0,0.0,0.0]
H=psr.system.create_atom(carts,1)
BI=psr.system.BasisInfo()
BI.shells=[FakeD,FakeD2]
H.basis_sets={"PRIMARY" :BI }
GhH=psr.system.make_ghost_atom(H)
Atoms=psr.system.AtomSetUniverse([H,GhH])
Mol=psr.system.System(Atoms,True)
BS=Mol.get_basis_set("PRIMARY")
BS2=psr.system.BasisSet(BS)


tester.test_value("Copy constructors work",BS,BS2)
BS3=psr.system.BasisSet(1,3,3,3)
tester.test("Inequality works",True,True,BS3.__ne__,BS2)

tester.test("Get types works",True,{cGTO,sGTO},BS.get_types)
tester.test("Get n shells",True,4,BS2.n_shell)
tester.test("Get n unique shells",True,2,BS.n_unique_shell)
tester.test("Get n primitives",True,12,BS2.n_primitives)
tester.test("Get n coeficients",True,12,BS2.n_coefficients)
tester.test("Get number of functions",True,22,BS.n_functions)
tester.test("Maximum number of primitivs",True,3,BS2.max_n_primitives)
tester.test("Max angular momentum",True,2,BS2.max_am)
tester.test("All angular momentum",True,{2},BS.all_am)
tester.test("Max n functions in a shell",True,6,BS2.max_n_functions)
tester.test("Shell start",True,6,BS2.shell_start,1)
tester.test("Invalid shell start",False,6,BS2.shell_start,99)
Si,Sj=BS.shell(3),BS.shell(2)
tester.test("Shell has right coordinates",True,carts,Si.get_coords)
tester.test("Shell has right coordinate",True,carts[1],Si.get_coord,1)
tester.test("Get invalid shell",False,Si,BS.shell,99)
tester.test("Get unique shell",True,Si,BS.unique_shell,1)
tester.test("Get invalid unique shell",False,Si,BS.unique_shell,99)
i=0
for Sk in BS2:
   tester.test("Iterator "+str(i),True, Sk,Si)
   i=i+1

tester.test("Get valid shell info",True,FakeD,BS2.shell_info,0)
tester.test("Get invalid shell info",False,FakeD,BS.shell_info,99)
BS4=psr.system.BasisSet(1,3,3,3)
tester.test("Add shell that fits",True,None,BS4.add_shell,FakeD,carts)
BS3.add_shell(FakeD,carts)
tester.test("Shrink to fit",True,BS3,BS4.shrink_fit)
tester.test("Add shell no fit",False,BS4.add_shell,FakeD2,carts)

da_hash=[134, 74, 207, 201, 97, 136, 34, 110, 135, 179, 103, 254, 210,25,248,91]
tester.test("Hash BS",True,da_hash,BS.my_hash)
tester.test("Hash BS2",True,da_hash,BS2.my_hash)


tester.print_results()
