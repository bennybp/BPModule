import pulsar as psr

def run_test():
    tester = psr.PyTester("Testing the BasisSet and BasisSetShell")
    cGTO = psr.ShellType.CartesianGaussian
    sGTO = psr.ShellType.SphericalGaussian
    alpha=[3.42525091, 0.62391373, 0.16885540]
    c=[0.15432897, 0.53532814, 0.44463454]

    FakeD=psr.BasisShellInfo(cGTO,2,3,1,alpha,c)
    FakeD2=psr.BasisShellInfo(sGTO,2,3,1,alpha,c)
    carts=[0.0,0.0,0.0]
    H=psr.create_atom(carts,1)
    BI=psr.BasisInfo()
    BI.shells=[FakeD,FakeD2]
    H.basis_sets={"PRIMARY" :BI }
    GhH=psr.make_ghost_atom(H)
    Atoms=psr.AtomSetUniverse([H,GhH])
    Mol=psr.System(Atoms,True)
    BS=Mol.get_basis_set("PRIMARY")
    BS2=psr.BasisSet(BS)


    tester.test_equal("Copy constructors work",BS,BS2)
    BS3=psr.BasisSet(1,3,3,3)
    tester.test_return("Inequality works",True,True,BS3.__ne__,BS2)

    tester.test_return("Get types works",True,{cGTO,sGTO},BS.get_types)
    tester.test_return("Get n shells",True,4,BS2.n_shell)
    tester.test_return("Get n unique shells",True,2,BS.n_unique_shell)
    tester.test_return("Get n primitives",True,12,BS2.n_primitives)
    tester.test_return("Get n coeficients",True,12,BS2.n_coefficients)
    tester.test_return("Get number of functions",True,22,BS.n_functions)
    tester.test_return("Maximum number of primitivs",True,3,BS2.max_n_primitives)
    tester.test_return("Max angular momentum",True,2,BS2.max_am)
    tester.test_return("All angular momentum",True,{2},BS.all_am)
    tester.test_return("Max n functions in a shell",True,6,BS2.max_n_functions)
    tester.test_return("Shell start",True,6,BS2.shell_start,1)
    tester.test_call("Invalid shell start",False,BS2.shell_start,99)
    Si,Sj=BS.shell(3),BS.shell(2)
    tester.test_return("Shell has right coordinates",True,carts,Si.get_coords)
    tester.test_return("Shell has right coordinate",True,carts[1],Si.get_coord,1)
    tester.test_call("Get invalid shell",False,BS.shell,99)
    tester.test_return("Get unique shell",True,Si,BS.unique_shell,1)
    tester.test_call("Get invalid unique shell",False,BS.unique_shell,99)
    i=0
    for Sk in BS2:
       tester.test_equal("Iterator "+str(i),Sk,Si if i%2==1 else Sj)
       i=i+1

    tester.test_return("Get valid shell info",True,FakeD,BS2.shell_info,0)
    tester.test_call("Get invalid shell info",False,FakeD,BS.shell_info,99)
    BS4=psr.BasisSet(1,3,3,3)
    tester.test_return("Add shell that fits",True,None,BS4.add_shell,FakeD,carts)
    BS3.add_shell(FakeD,carts)
    tester.test_return("Shrink to fit",True,BS3,BS4.shrink_fit)
    tester.test_call("Add shell no fit",False,BS4.add_shell,FakeD2,carts)

    tester.test_return("Hash BS",True,BS.my_hash(),BS2.my_hash)

    tester.print_results()
    return tester.nfailed()
