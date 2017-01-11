#include <pulsar/testing/CppTester.hpp>
#include <pulsar/datastore/CacheData.hpp>

using namespace pulsar;
using namespace std;

TEST_SIMPLE(TestCacheData){
    CppTester tester("Testing CacheData class");

    CacheMap cm1;
    using Vector=vector<double>;
    Vector v1({1.0,2.0,3.0}),v2({2.0,3.0,4.0});
    const std::string key="Vector1",not_key="Not A key";
    unsigned int policy=CacheData::NoPolicy;

    CacheData cd1(&cm1,"my module");
    cd1.set(key,v1,policy);
    set<string> keys({"my module%%"+key});
    tester.test_member_return("size",true,1,&CacheData::size,&cd1);
    tester.test_member_return("get_keys",true,keys,&CacheData::get_keys,&cd1);
    shared_ptr<const Vector> pv1=cd1.get<Vector>(key,false),pv2;
    tester.test_equal("get valid key works",v1,*pv1);
    tester.test_member_return("get invalid key works",true,pv2,&CacheData::get<Vector>,&cd1,not_key,false);
    cd1.set(key,v2,policy);
    pv1=cd1.get<Vector>(key,false);
    tester.test_equal("set overwrites",v2,*pv1);
    tester.test_member_call("erase",true,&CacheData::erase,&cd1,key);
    tester.test_member_return("erase works",true,0,&CacheData::size,&cd1);

    tester.print_results();
    return tester.nfailed();
}
