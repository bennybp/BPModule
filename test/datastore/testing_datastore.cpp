#include<pulsar/util/Pybind11.hpp>
#include<pulsar/datastore/CacheData.hpp>

using namespace std;
using namespace pulsar;

shared_ptr<CacheMap> cm1(make_shared<CacheMap>());

//Makes a CacheData object that can be tested in Python
CacheData get_cachedata(){
    return CacheData(cm1.get(),"my_module");
}


PYBIND11_PLUGIN(testing_datastore)
{
    pybind11::module mtop("testing_datastore", "Functions used to test datastore");
    mtop.def("get_cachedata",&get_cachedata);
    return mtop.ptr();
}
