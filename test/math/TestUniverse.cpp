#include <pulsar/testing/CppTester.hpp>
#include <pulsar/math/Universe.hpp>

using namespace pulsar;
using Universe_t=pulsar::Universe<double>;
using std::bind;
using namespace std::placeholders;

TEST_SIMPLE(TestUniverse){
    
    CppTester tester("Testing Universe Class C++ Interface");
    
    //Constructors and assignment
    Universe_t U0,U1;
    tester.test_equal("Default constructor",U0,U1);
    Universe_t U2(1.0,2.0,3.0),U3={1.0,2.0,3.0};
    tester.test_equal("Variadic and initializer construction",U2,U3);
    Universe_t U4(U3);
    tester.test_equal("Copy constructor",U3,U4);
    Universe_t U5(std::move(U4));
    tester.test_equal("Move constructor",U3,U5);
    U0=U2;
    tester.test_equal("Assignment",U0,U3);
    U1=std::move(U2);
    tester.test_equal("Move assignment",U0,U1);
    
    //Don't use U2 or U4 after this point
    
    //Basic properties
    tester.test_return("Size",true,3,bind(&Universe_t::size,&U0));
    tester.test_return("Count",true,true,bind(&Universe_t::count,&U0,1.0));
    tester.test_return("Get index",true,2,bind(&Universe_t::idx,&U3,3.0));
    tester.test_member_call("Get non-existant index",false,
                            &Universe_t::idx,&U3,5.0);

    tester.test_equal("Get hash U0",U0.my_hash(),U1.my_hash());
    tester.test_equal("Get hash U1",U0.my_hash(),U3.my_hash());
    tester.test_equal("Get hash U3",U0.my_hash(),U5.my_hash());

    
    //Element access/modification
    std::vector<double> elems(U0.begin(),U0.end()),right_elems={1.0,2.0,3.0};
    tester.test_equal("Iterators",elems,right_elems);
    auto idxop=bind(&Universe_t::operator[],_1,_2);
    tester.test_return("Subscript operator",true,3.0,idxop,U0,2);
    tester.test_call("Subscript operator (out of range)",false,idxop,U0,9);
    auto atop=bind(&Universe_t::at,_1,_2);
    tester.test_return("at function",true,3.0,atop,U0,2);
    tester.test_call("at function (out of range)",false,atop,U0,9);
    U0.insert(4.0);
    auto inop=bind(static_cast<Universe_t&(Universe_t::*)(const double&)>
        (&Universe_t::insert),_1,_2);
    tester.test_return("insert elements",true,U0,inop,U1,4.0);
    double f1=5.0,f2=5.0;
    U1.insert(std::move(f1));
    auto inop2=bind(static_cast<Universe_t&(Universe_t::*)(double&&)>
        (&Universe_t::insert),_1,_2);
    tester.test_return("move insert",true,U1,inop2,U0,std::move(f2));
    std::array<double,5> values={1.0,2.0,3.0,4.0,5.0};
    using Itr_t=std::array<double,5>::iterator;
    Universe_t U6;
    auto inop3=bind(static_cast<Universe_t&(Universe_t::*)(Itr_t,Itr_t)>
    (&Universe_t::insert),_1,_2,_3);
    tester.test_return("range insert",true,U1,inop3,U6,values.begin(),values.end());
    
    //Set operations
    Universe_t U7={4.0,5.0,6.0},U8={1.0,2.0,3.0,4.0,5.0,6.0};
    using Assign_t1=Universe_t&(Universe_t::*)(const Universe_t&);
    using Assign_t2=Universe_t&(Universe_t::*)(Universe_t&&);
    using Op_t1=Universe_t(Universe_t::*)(const Universe_t&)const;
    using Op_t2=Universe_t(Universe_t::*)(Universe_t&&)const;
    
    auto uass=bind(static_cast<Assign_t1>(&Universe_t::union_assign),_1,_2);
    tester.test_return("union assign",true,U8,uass,U6,U7);
    auto uass2=bind(static_cast<Assign_t2>(&Universe_t::union_assign),_1,_2);
    tester.test_return("union assign move",true,U8,uass2,U5,std::move(U7));
    auto uass3=bind(static_cast<Op_t1>(&Universe_t::set_union),_1,_2);
    tester.test_return("union",true,U8,uass3,U1,U6);
    auto uass4=bind(static_cast<Op_t2>(&Universe_t::set_union),_1,_2);
    tester.test_return("union",true,U8,uass4,U1,std::move(U6));
    auto iass=bind(&Universe_t::intersection_assign,_1,_2);
    Universe_t U9={1.0,2.0},U10={1.0,2.0,15.0,16.0};
    tester.test_return("intersection assign",true,U9,iass,U1,U10);
    auto iass2=bind(&Universe_t::intersection,_1,_2);
    tester.test_return("intersection",true,U9,iass2,U5,U10);
    auto dass=bind(&Universe_t::difference_assign,_1,_2);
    Universe_t U11={3.0,4.0,5.0,6.0};
    tester.test_return("difference assign",true,U11,dass,U5,U9);
    auto dass2=bind(&Universe_t::difference,_1,_2);
    tester.test_return("difference",true,U11,dass2,U8,U9);
    
    //Comparison operators
    tester.test_return("not equal",true,true,bind(&Universe_t::operator!=,_1,_2),U9,U11);
    auto supset=bind(&Universe_t::is_superset_of,_1,_2);
    tester.test_return("superset equal",true,true,supset,U11,U5);
    tester.test_return("superset true",true,true,supset,U8,U11);
    tester.test_return("superset false",true,false,supset,U8,U10);
    auto supset2=bind(&Universe_t::is_proper_superset_of,_1,_2);
    tester.test_return("proper superset equal",true,false,supset2,U11,U5);
    tester.test_return("proper supserset true",true,true,supset2,U8,U11);
    tester.test_return("proper superset false",true,false,supset2,U8,U10);
    auto subset=bind(&Universe_t::is_subset_of,_1,_2);
    tester.test_return("subset equal",true,true,subset,U11,U5);
    tester.test_return("subset true",true,true,subset,U11,U8);
    tester.test_return("subset false",true,false,subset,U10,U8);
    auto subset2=bind(&Universe_t::is_proper_subset_of,_1,_2);
    tester.test_return("proper subset equal",true,false,subset2,U11,U5);
    tester.test_return("proper subset true",true,true,subset2,U11,U8);
    tester.test_return("proper subset false",true,false,subset2,U10,U8);   
    
    tester.print_results();
    return tester.nfailed();
    
}

