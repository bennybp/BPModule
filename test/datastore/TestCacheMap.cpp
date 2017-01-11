#include <pulsar/testing/CppTester.hpp>
#include <pulsar/datastore/CacheMap.hpp>

using namespace pulsar;
using namespace std;

TEST_SIMPLE(TestCacheMap){
    CppTester tester("Testing CacheMap class");

    CacheMap cm1;
    using Vector=vector<double>;
    Vector v1({1.0,2.0,3.0}),v2({2.0,3.0,4.0});
    const std::string key="Vector1",not_key="Not A key";
    unsigned int policy=CacheMap::NoPolicy;
    cm1.set(key,v1,policy);
    const set<string> keys({key});
    tester.test_member_return("get_keys",true,keys,&CacheMap::get_keys,&cm1);
    shared_ptr<const int> pi;
    tester.test_member_return("get w/non-existent key",true,pi,&CacheMap::get<int>,&cm1,not_key,false);
    auto pj=cm1.get<Vector>(key,false);
    tester.test_equal("get_w/real key",v1,*pj);
    tester.test_member_return("size",true,1,&CacheMap::size,&cm1);
    cm1.set(key,v2,policy);

    pj=cm1.get<Vector>(key,false);
    tester.test_equal("set overwrote",v2,*pj);
    tester.test_member_call("erase w/non-existent key",true,&CacheMap::erase,&cm1,not_key);
    tester.test_member_return("erase did nothing",true,1,&CacheMap::size,&cm1);
    tester.test_member_call("erase w/real key",true,&CacheMap::erase,&cm1,key);
    tester.test_member_return("erased",true,0,&CacheMap::size,&cm1);
    tester.test_member_call("clear",true,&CacheMap::clear,&cm1);
    tester.test_member_return("cleared",true,0,&CacheMap::size,&cm1);

    tester.print_results();
    return tester.nfailed();
}
