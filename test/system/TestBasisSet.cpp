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
    
    tester.test_member_return("Get types works",true,ShellSet({cGTO,sGTO}),
                              &BasisSet::get_types,&BS);
    tester.test_member_return("Get n shells",true,4,&BasisSet::n_shell,&BS2);
    tester.test_member_return("Get n unique shells",true,2,
                              &BasisSet::n_unique_shell,&BS);
    tester.test_member_return("Get n primitives",true,12,
                              &BasisSet::n_primitives,&BS3);
    tester.test_member_return("Get n coeficients",true,12,
                              &BasisSet::n_coefficients,&BS2);
    tester.test_member_return("Get number of functions",true,22,
                              &BasisSet::n_functions,&BS);
    tester.test_member_return("Maximum number of primitivs",true,3,
                              &BasisSet::max_n_primitives,&BS3);
    tester.test_member_return("Max angular momentum",true,2,&BasisSet::max_am,&BS2);
    std::set<int> am({2});
    tester.test_member_return("All angular momentum",true,am,&BasisSet::all_am,&BS);
    tester.test_member_return("Max n functions in a shell",true,6,
                              &BasisSet::max_n_functions,&BS3);
    tester.test_member_return("Shell start",true,6,&BasisSet::shell_start,&BS2,1);
    tester.test_member_call("Invalid shell start",false,&BasisSet::shell_start,
                            &BS2,99);
    const BasisSetShell& Si=BS.shell(3);
    const BasisSetShell& Sj=BS.shell(2);
    tester.test_member_return("Shell has right coordinates",true,carts,
                              &BasisSetShell::get_coords,&Si);
    tester.test_member_return("Shell has right coordinate",true,carts[1],
        &BasisSetShell::get_coord,&Si,1);
    tester.test_member_call("Get invalid shell",false,&BasisSet::shell,&BS,99);
    tester.test_member_call("Get unique shell",true,&BasisSet::unique_shell,&BS,1);
    tester.test_member_call("Get invalid unique shell",false,&BasisSet::unique_shell,&BS,99);
    size_t counter=0;
    for(const BasisSetShell& Sk: BS2)
    {
       tester.test_equal("Iterator "+std::to_string(counter),true, Sk==(counter%2?Si:Sj));
       ++counter;
    }
    
    
    tester.test_member_return("Get valid shell info",true,FakeD,
                              &BasisSet::shell_info,&BS3,0);
    tester.test_member_call("Get invalid shell info",false,
                            &BasisSet::shell_info,&BS3,99);
    BasisSet BS4(1,3,3,3),BS5(1,3,3,3);
    tester.test_member_call("Add shell that fits",true,&BasisSet::add_shell,
                            &BS4,FakeD,carts);
    BS5.add_shell(FakeD,carts);
    tester.test_member_return("Shrink to fit",true,BS5,&BasisSet::shrink_fit,&BS4);
    tester.test_member_call("Add shell no fit",false,&BasisSet::add_shell,&BS4,
                            FakeD2,carts);

    tester.test_equal("Hash BS",BS.my_hash(),BS2.my_hash());
    tester.test_equal("Hash BS2",BS.my_hash(),BS3.my_hash());
    
    tester.print_results();
    return tester.nfailed();
}

