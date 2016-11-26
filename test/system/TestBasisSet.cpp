#include <pulsar/testing/CppTester.hpp>
#include <pulsar/system/System.hpp>

using namespace pulsar;
using ShellSet=std::set<ShellType>;

TEST_SIMPLE(TestBasisSet){

    ShellType cGTO = ShellType::CartesianGaussian;
    ShellType sGTO = ShellType::SphericalGaussian;
    //RMR- It is my understanding that BSS is really only going to be made by BS
    CppTester tester("Testing BasisSet and BasisSetShell");

    std::vector<double> alpha({3.42525091, 0.62391373, 0.16885540}),
    c({0.15432897, 0.53532814, 0.44463454});
    
    BasisShellInfo FakeD(cGTO, 2, 3, 1, alpha, c),
                  FakeD2(sGTO, 2, 3, 1, alpha, c);
    std::array<double,3> carts({0.0,0.0,0.0});
    Atom H=create_atom(carts,1);
    H.basis_sets["PRIMARY"].shells.push_back(FakeD);
    H.basis_sets["PRIMARY"].shells.push_back(FakeD2);
    Atom GhH=make_ghost_atom(H);
    AtomSetUniverse Atoms({H,GhH});
    System Mol(Atoms,true);
    BasisSet BS(std::move(Mol.get_basis_set("PRIMARY")));
    BasisSet BS2(BS);   
    tester.test_equal("Move and copy constructors work",BS,BS2);
    
    BasisSet BS3(2,6,6,6);
    tester.test_equal("Inequality works",true,BS3!=BS2);
    BS3=std::move(BS2);
    tester.test_equal("Move assignment works",BS3,BS);
    BS2=BS3;
    tester.test_equal("Copy assignment works",BS2,BS);
    
    //All basis sets up to BS3 should be equal at this point
    
    TEST_FXN("Get types works",true,ShellSet({cGTO,sGTO}),BS.get_types());
    TEST_FXN("Get n shells",true,4,BS2.n_shell());
    TEST_FXN("Get n unique shells",true,2,BS.n_unique_shell());
    TEST_FXN("Get n primitives",true,12,BS3.n_primitives());
    TEST_FXN("Get n coeficients",true,12,BS2.n_coefficients());
    TEST_FXN("Get number of functions",true,22,BS.n_functions());
    TEST_FXN("Maximum number of primitivs",true,3,BS3.max_n_primitives());
    TEST_FXN("Max angular momentum",true,2,BS2.max_am());
    std::set<int> am({2});
    TEST_FXN("All angular momentum",true,am,BS.all_am());
    TEST_FXN("Max n functions in a shell",true,6,BS3.max_n_functions());
    TEST_FXN("Shell start",true,6,BS2.shell_start(1));
    TEST_FXN("Invalid shell start",false,6,BS2.shell_start(99));
    const BasisSetShell& Si=BS.shell(3);
    const BasisSetShell& Sj=BS.shell(2);
    TEST_FXN("Shell has right coordinates",true,carts,Si.get_coords());
    TEST_FXN("Shell has right coordinate",true,carts[1],Si.get_coord(1));
    TEST_FXN("Get invalid shell",false,Si,BS.shell(99));
    TEST_FXN("Get unique shell",true,Si,BS.unique_shell(1));
    TEST_FXN("Get invalid unique shell",false,Si,BS.unique_shell(99));
    size_t counter=0;
    for(const BasisSetShell& Sk: BS2){
        //if(counter%2)//No ternary b/c of its stupid return type
            tester.test_equal("Iterator "+std::to_string(counter),true, Sk==(counter%2?Si:Sj));
        //else
       //     tester.test_equal("Iterator "+std::to_string(counter),true,Sk==Sj);
        ++counter;
    }
    
    
    TEST_FXN("Get valid shell info",true,FakeD,BS3.shell_info(0));
    TEST_FXN("Get invalid shell info",false,FakeD,BS3.shell_info(99));
    BasisSet BS4(1,3,3,3),BS5(1,3,3,3);
    TEST_VOID("Add shell that fits",true,BS4.add_shell(FakeD,carts));
    BS5.add_shell(FakeD,carts);
    TEST_FXN("Shrink to fit",true,BS5,BS4.shrink_fit());
    TEST_VOID("Add shell no fit",false,BS4.add_shell(FakeD2,carts));

    TEST_FXN("Hash BS",true,BS.my_hash(),BS2.my_hash());
    TEST_FXN("Hash BS2",true,BS.my_hash(),BS3.my_hash());   
    
    tester.print_results();
    return tester.nfailed();
}

