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
    tester.test_function("Inequality works",True,True,BS3.__ne__,BS2)

    tester.test_function("Get types works",True,{cGTO,sGTO},BS.get_types)
    tester.test_function("Get n shells",True,4,BS2.n_shell)
    tester.test_function("Get n unique shells",True,2,BS.n_unique_shell)
    tester.test_function("Get n primitives",True,12,BS2.n_primitives)
    tester.test_function("Get n coeficients",True,12,BS2.n_coefficients)
    tester.test_function("Get number of functions",True,22,BS.n_functions)
    tester.test_function("Maximum number of primitivs",True,3,BS2.max_n_primitives)
    tester.test_function("Max angular momentum",True,2,BS2.max_am)
    tester.test_function("All angular momentum",True,{2},BS.all_am)
    tester.test_function("Max n functions in a shell",True,6,BS2.max_n_functions)
    tester.test_function("Shell start",True,6,BS2.shell_start,1)
    tester.test_function("Invalid shell start",False,6,BS2.shell_start,99)
    Si,Sj=BS.shell(3),BS.shell(2)
    tester.test_function("Shell has right coordinates",True,carts,Si.get_coords)
    tester.test_function("Shell has right coordinate",True,carts[1],Si.get_coord,1)
    tester.test_function("Get invalid shell",False,Si,BS.shell,99)
    tester.test_function("Get unique shell",True,Si,BS.unique_shell,1)
    tester.test_function("Get invalid unique shell",False,Si,BS.unique_shell,99)
    i=0
    for Sk in BS2:
       tester.test_equal("Iterator "+str(i),Sk,Si if i%2==1 else Sj)
       i=i+1

    tester.test_function("Get valid shell info",True,FakeD,BS2.shell_info,0)
    tester.test_function("Get invalid shell info",False,FakeD,BS.shell_info,99)
    BS4=psr.BasisSet(1,3,3,3)
    tester.test_function("Add shell that fits",True,None,BS4.add_shell,FakeD,carts)
    BS3.add_shell(FakeD,carts)
    tester.test_function("Shrink to fit",True,BS3,BS4.shrink_fit)
    tester.test_function("Add shell no fit",False,BS4.add_shell,FakeD2,carts)

    tester.test_function("Hash BS",True,BS.my_hash(),BS2.my_hash)

    tester.print_results()
    return tester.nfailed()
