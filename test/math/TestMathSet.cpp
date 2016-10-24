#include "TestCXX.hpp"
#include <pulsar/math/MathSet.hpp>

using Universe_t=pulsar::math::Universe<double>;
using MathSet_t=pulsar::math::MathSet<double>;
using std::bind;
using namespace std::placeholders;

TEST_CLASS(TestMathSet){
    Tester tester("Testing MathSet and MathSet Iterator C++ Interface");
    
    std::shared_ptr<Universe_t> U1=std::make_shared<Universe_t>(1.0,2.0,3.0),
            U2=std::make_shared<Universe_t>(3.0,4.0,5.0),
            U3=std::make_shared<Universe_t>(3.0);
    
    
    //Constructors and assignment
    MathSet_t S1(U1,std::set<size_t>({2}));
    MathSet_t S2(S1);
    tester.test("Constructor 1 and copy constructor works",S1,S2);
    U1->insert(4.0);
    tester.test("Shallow copy of universe",S1,S2);
    MathSet_t S3(U2,true),S4(U2,false);
    tester.test("Fill constructor 2nd argument test",false,S3==S4);
    MathSet_t S5(U2,{0,1,2});
    tester.test("Fill constructor works",S3,S5);
    MathSet_t S6(std::move(S5));
    tester.test("Move constructor work",S3,S6);
    S4=S6;
    tester.test("Assignment constructor works",S4,S6);
    MathSet_t S7=std::move(S3);
    tester.test("Move assignment constructor works",S7,S4);
    MathSet_t S8=S2.clone();
    U1->insert(5.0);
    tester.test("Clone is not aliased",false,S8==S2);    
    MathSet_t::const_iterator Itr(S8.begin());
    MathSet_t::const_iterator Itr2=Itr;
    tester.test("iterator copy and assignment works",Itr,Itr2);
    MathSet_t::const_iterator Itr3(std::move(Itr2)),Itr4=std::move(Itr);
    tester.test("iterator move constructor works",Itr3,Itr4);  
    
    //Access
    tester.test("Get universe works",U1,S1.get_universe());
    tester.test("As universe",*U3,S1.as_universe());
    tester.test("Size works",1,S8.size());
    tester.test("count element false",false,S4.count(15.0));
    tester.test("count element true",true,S6.count(3.0));
    tester.test("count index false",false,S7.count_idx(15));
    tester.test("count index true",true,S8.count_idx(2));
    std::vector<double> vals({3.0,4.0,5.0}),itercheck(S7.begin(),S7.end());
    tester.test("iterators work",vals,itercheck);
    auto idx_fxn=bind(&MathSet_t::idx,_1,_2);
    tester.test("idx valid",true,2,idx_fxn,S8,3.0);
    tester.test("idx invalid",false,2,idx_fxn,S8,55.0);
    auto idx_fxn2=bind(&MathSet_t::insert,_1,_2);
    MathSet_t S11(U1,{2,3});
    tester.test("insert by valid elem",true,S11,idx_fxn2,S1,4.0);
    tester.test("insert by invalid elem",false,S11,idx_fxn2,S1,55.0);
    auto idx_fxn3=bind(&MathSet_t::insert_idx,_1,_2);
    tester.test("insert by valid index",true,S11,idx_fxn3,S2,3);
    tester.test("insert by invalid index",false,S11,idx_fxn3,S2,99);
    tester.test("Iterator access",3.0,*Itr3);
    MathSet_t::const_iterator Itr5=S7.begin();
    tester.test("Iterator postfix value",3.0,*Itr5++);
    tester.test("Iterator postfix increment",4.0,*Itr5);
    tester.test("Iterator prefix increment",5.0,*++Itr5);
    tester.test("Iterator non-equlity",true,Itr5!=S7.end());
    
    //Set operations
    MathSet_t S9(U1,{1,2,3}),S10(U1,{1,2,4}),S12(U1,{1,2}),
              S13(U1,std::set<size_t>({3}));
    auto un1=bind(&MathSet_t::union_assign,_1,_2);
    auto un2=bind(&MathSet_t::set_union,_1,_2);
    tester.test("union",true,S9,un2,S1,S12);
    tester.test("union fail",false,S9,un2,S1,S4);
    tester.test("union assign",true,S9,un1,S1,S12);
    tester.test("union assign fail",false,S9,un1,S1,S4);
    auto int1=bind(&MathSet_t::intersection_assign,_1,_2);
    auto int2=bind(&MathSet_t::intersection,_1,_2);
    tester.test("intersection",true,S12,int2,S1,S10);
    tester.test("intersection fail",false,S2,int2,S1,S4);
    tester.test("intersection assign",true,S12,int1,S1,S10);
    tester.test("intersection assign fail",false,S2,int1,S1,S4);
    auto diff1=bind(&MathSet_t::difference_assign,_1,_2);
    auto diff2=bind(&MathSet_t::difference,_1,_2);
    tester.test("difference",true,S13,diff2,S2,S12);
    tester.test("difference fail",false,S13,diff2,S2,S4);
    tester.test("difference assign",true,S13,diff1,S2,S12);
    tester.test("difference assign fail",false,S13,diff1,S2,S4);
    MathSet_t S14(U1,true);
    S14-=S2;
    tester.test("complement",S14,S2.complement());
    
    //Set comparisons
    auto subs=bind(&MathSet_t::is_subset_of,_1,_2);
    tester.test("subset equal",true,true,subs,S2,S13);
    tester.test("subset true",true,true,subs,S2,S9);
    tester.test("subset false",true,false,subs,S9,S2);
    auto subs2=bind(&MathSet_t::is_proper_subset_of,_1,_2);
    tester.test("proper subset equal",true,false,subs2,S2,S13);
    tester.test("proper subset true",true,true,subs2,S2,S9);
    tester.test("proper subset false",true,false,subs2,S9,S2);
    auto sups=bind(&MathSet_t::is_superset_of,_1,_2);
    tester.test("superset equal",true,true,subs,S2,S13);
    tester.test("superset true",true,true,sups,S9,S2);
    tester.test("superset false",true,false,sups,S2,S9);
    auto sups2=bind(&MathSet_t::is_proper_superset_of,_1,_2);
    tester.test("proper superset equal",true,false,sups2,S2,S13);
    tester.test("proper superset true",true,true,sups2,S9,S2);
    tester.test("proper superset false",true,false,sups2,S2,S9);
    tester.test("not equal",true,S2!=S14);
    
    
    std::vector<double> transresults({4.0,6.0});
    MathSet_t NewS=S1.transform([](const double& in){return 2.0*in;});
    std::vector<double> tresults2(NewS.begin(),NewS.end());
    tester.test("transform works",transresults,tresults2);
    
    MathSet_t NewS2=S1.partition([](const double& in){return in==2.0;});
    std::vector<double> partresults({2.0}),presults2(NewS2.begin(),NewS2.end());
    tester.test("partition works",partresults,presults2);
    
    const std::string Corr_Hash="6c146c3b32a8c3dceed476a2279c3503";
    auto hash=bind(&bphash::hash_to_string,bind(&MathSet_t::my_hash,_1));
    tester.test("hash works check 1",true,Corr_Hash,hash,S2);
    tester.test("hash works check 2",true,Corr_Hash,hash,S13);
    
    tester.print_results();
}
