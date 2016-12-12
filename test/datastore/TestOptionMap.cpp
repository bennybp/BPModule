#include <pulsar/testing/CppTester.hpp>
#include <pulsar/datastore/OptionMap.hpp>

using namespace pulsar;


TEST_SIMPLE(TestOptionMap){
 CppTester tester("Testing OptionMap class");

 OptionMap OM1,OM2;
 OM1.add_option("Option 1",Int,false,pybind11::none(),)

 tester.print_results();
 return tester.nfailed();
}

