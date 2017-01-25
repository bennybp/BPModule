#include <pulsar/testing/CppTester.hpp>
#include <pulsar/util/Serialization.hpp>
#include <pulsar/util/Pybind11.hpp>
using namespace pulsar;
using namespace detail;
using namespace std;

TEST_SIMPLE(TestSerialization){
    CppTester tester("Testing Serialization");

    using stream_t=StdStreamArchive<stringstream>;
    shared_ptr<stream_t> ar=make_shared<MemoryArchive>();
    shared_ptr<stream_t> ar2=make_shared<MemoryArchive>();
    shared_ptr<stream_t> ar3=make_shared<MemoryArchive>();
    double in_val=2.0,out_val;
    const size_t size=sizeof(double);
    tester.test_member_call("Can't serialize without starting",false,&stream_t::serialize<double>,ar.get(),in_val);
    tester.test_member_call("Start serialize",true,&stream_t::begin_serialization,ar.get());
    tester.test_member_call("Can't start serialize while serializing",false,&stream_t::begin_serialization,ar.get());
    tester.test_member_call("Can't start unserialize while serializing",false,&stream_t::begin_unserialization,ar.get());
    tester.test_member_call("Can't unserialize while serializing",false,&stream_t::unserialize<double>,ar.get(),out_val);
    tester.test_member_call("Can't stop unserializing while serializing",false,&stream_t::end_unserialization,ar.get());
    tester.test_member_call("Serialize",true,&stream_t::serialize<double>,ar.get(),in_val);
    tester.test_member_call("Stop serializing",true,&stream_t::end_serialization,ar.get());

    tester.test_member_return("Size of archive",true,size,&stream_t::size,ar.get());
    stringstream ss;
    ss.write(reinterpret_cast<const char*>(&in_val),size);
    ByteArray corr_data(size);
    ss.read(corr_data.data(),size);
    tester.test_member_return("To byte array",true,corr_data,&stream_t::to_byte_array,ar.get());

    tester.test_member_call("Can't unserialize without starting",false,&stream_t::unserialize<double>,ar.get(),out_val);
    tester.test_member_call("Start unserialize",true,&stream_t::begin_unserialization,ar.get());
    tester.test_member_call("Can't start unserialize while unserializing",false,&stream_t::begin_unserialization,ar.get());
    tester.test_member_call("Can't start serialize while unserializing",false,&stream_t::begin_serialization,ar.get());
    tester.test_member_call("Can't serialize while unserializing",false,&stream_t::serialize<double>,ar.get(),in_val);
    tester.test_member_call("Can't stop serializing while unserializing",false,&stream_t::end_serialization,ar.get());
    tester.test_member_call("Unserialization",true,&stream_t::unserialize<double>,ar.get(),out_val);
    tester.test_member_call("Stop unserializing",true,&stream_t::end_unserialization,ar.get());

    using from_array1=void(stream_t::*)(const ByteArray&);
    using from_array2=void(stream_t::*)(const char*,size_t);
    tester.test_member_call("From byte array",true,
        static_cast<from_array1>(&stream_t::from_byte_array),ar2.get(),corr_data);
    tester.test_member_return("From byte array worked",true,size,&stream_t::size,ar2.get());
    tester.test_member_call("From byte array pointer",true,
        static_cast<from_array2>(&stream_t::from_byte_array),ar3.get(),corr_data.data(),sizeof(double));
    tester.test_member_return("From byte array pointer worked",true,size,&stream_t::size,ar3.get());


    pybind11::list a_list;
    a_list.append(pybind11::int_(1));
    a_list.append(pybind11::int_(2));
    list<int> c_list=a_list.cast<list<int>>();
    ByteArray serialized_python=to_byte_array(a_list);
    pybind11::list b_list=from_byte_array<pybind11::list>(serialized_python);

    tester.test_equal("Python serialized/deserialized correctly",c_list,b_list.cast<list<int>>());

    tester.test_equal("Can serialize a pybind11 object",true,SerializeCheck<pybind11::object>::value);
    tester.test_equal("Can serialize shared_ptr",true,SerializeCheck<shared_ptr<double>>::value);
    tester.test_equal("Can serialize unique_ptr",true,SerializeCheck<unique_ptr<double>>::value);
    tester.test_equal("Can serialize vector",true,SerializeCheck<vector<double>>::value);
    tester.test_equal("Can serialize set",true,SerializeCheck<set<double>>::value);
    tester.test_equal("Can serialize map",true,SerializeCheck<map<int,double>>::value);

    tester.print_results();
    return tester.nfailed();
}
