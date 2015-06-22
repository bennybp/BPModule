#include "BPModule/core/ModuleStore.h"

// All the module base classes
#include "BPModule/modulebase/All.h"

// python stuff
#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {


////////////////////////////
// Main boost python part
////////////////////////////
BOOST_PYTHON_MODULE(bpmodule_core)
{
    // add some handlers for GetModule for the module base classes
    class_<ModuleStore, boost::noncopyable>("ModuleStore")
           .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_value_policy<manage_new_object>());


    // export the module base classes
    class_<Test_Base, bases<ModuleBase>, boost::noncopyable>("Test_Base", no_init)
           .def("RunTest", &Test_Base::RunTest);
}



} // close namespace bpmodule

