// helpers
#include "BPModule/python_helper/Python_stdconvert.hpp"

// Parallelization
#include "BPModule/parallel/Parallel.hpp"

// Various components
#include "BPModule/modulestore/ModuleStore.hpp"
#include "BPModule/modulestore/CModuleLoader.hpp"
#include "BPModule/modulestore/PyModuleLoader.hpp"
#include "BPModule/exception/Exception.hpp"
#include "BPModule/modulebase/All_python.hpp"


using namespace boost::python;
namespace bpy = boost::python;

using bpmodule::modulestore::ModuleInfo;
using bpmodule::modulestore::ModuleStore;
using bpmodule::modulebase::ModuleBase;
using bpmodule::modulebase::Test_Base;
using bpmodule::modulestore::CModuleLoader;
using bpmodule::modulestore::PyModuleLoader;

namespace bpmodule {
namespace export_python {


template<typename T>
boost::shared_ptr<T> Wrap_GetScopedModule(ModuleStore * ms, const std::string & key)
{
    T & mod = ms->GetModule<T>(key);
    std::function<void(ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(ModuleBase *)>(&ModuleStore::RemoveModule), ms, std::placeholders::_1);
    return boost::shared_ptr<T>(&mod, dfunc);
}


////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(bpmodule_modulestore)
{
    // This is only needed because we pass through python
    // No need to do declare all the members, etc 
    class_<ModuleInfo, boost::noncopyable>("ModuleInfo");


    class_<ModuleStore, boost::noncopyable>("ModuleStore")
    .def("Size", &ModuleStore::Size)
    .def("Has", &ModuleStore::Has)
    .def("RemoveModule", static_cast<void(ModuleStore::*)(unsigned long)>(&ModuleStore::RemoveModule))
    .def("SetOptions", bpmodule::modulestore::export_python::Wrap_ModuleStore_SetOptions)
    .def("GetKeys", &ModuleStore::GetKeys)
    .def("KeyInfo", &ModuleStore::KeyInfo)
    .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_internal_reference<>())
    .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_internal_reference<>())
    .def("GetScopedModule", Wrap_GetScopedModule<ModuleBase>)
    .def("GetScopedModule_Test", Wrap_GetScopedModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleStore *>())
    .def("CloseHandles", &CModuleLoader::CloseHandles)
    .def("UnloadAll", &CModuleLoader::UnloadAll)
    .def("LoadSO", bpmodule::modulestore::export_python::Wrap_CModuleLoader_LoadSO);

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleStore *>())
    .def("UnloadAll", &PyModuleLoader::UnloadAll)
    .def("AddPyModule", bpmodule::modulestore::export_python::Wrap_PyModuleLoader_AddPyModule);


    /////////////////////////
    // CalcData
    /////////////////////////
    class_<CalcData>("CalcData", init<>())
    .def(init<const CalcData &>())
    .def("Has", &CalcData::Has)
    .def("GetCopy", &CalcData::GetCopy<bpy::object>)
//    .def("GetRef", &CalcData::GetRef<bpy::object>, return_value_policy<copy_const_reference>()) 
    .def("Set", &CalcData::Set<bpy::object>)
    .def("Erase", &CalcData::Erase)
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &, const std::string &)>(&CalcData::SetRef))
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &)>(&CalcData::SetRef));
}


} // close namespace export_python
} // close namespace bpmodule

