#include "TestCXX.hpp"
#include <pulsar/math/PowerSetItr.hpp>

using Comb_t=std::vector<int>;
using PSItr_t=pulsar::math::PowerSetItr<Comb_t>;

TEST_CLASS(TestPowerSetItr){
    Tester tester("Testing PowerSetItr Class C++ Interface");
    
    Comb_t Com1={1,2,3},Com2;
    std::vector<Comb_t> Corr1={{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}};
    
    PSItr_t PSItr(Com2);
    std::vector<Comb_t> Combinations,Combinations1;
    while(PSItr)Combinations.push_back(*PSItr++);
    tester.test("Handles empty set",0,Combinations.size());
    
    PSItr_t PSItr2(Com1);
    PSItr_t PSItr3(PSItr2);
    while(PSItr2)Combinations.push_back(*PSItr2++);
    tester.test("Handles P({1,2,3})",Corr1,Combinations);
    
    while(PSItr3)Combinations1.push_back(*PSItr3++);
    tester.test("Copy works",Corr1,Combinations1);
    
    
    PSItr_t PSItr4(Com1,3,3);
    while(PSItr4){
        tester.test("Can use member functions of combinations",3,PSItr4->size());
        tester.test("Done() works",false,PSItr4.done());
        tester.test("Correctly handles max==min",Corr1.back(),*PSItr4);
        ++PSItr4;
    }
    
    tester.print_results();
}


