// Various components
#include "bpmodule/modulebase/All_python.hpp"
#include "bpmodule/modulestore/ModuleStore.hpp"
#include "bpmodule/modulestore/CModuleLoader.hpp"
#include "bpmodule/modulestore/PyModuleLoader.hpp"


using bpmodule::modulebase::ModuleBase;
using bpmodule::modulebase::Test_Base;
using namespace boost::python;


namespace bpmodule {
namespace modulestore {
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

BOOST_PYTHON_MODULE(modulestore)
{
    // This is only needed because we pass through python
    // No need to do declare all the members, etc 
    class_<ModuleInfo, boost::noncopyable>("ModuleInfo");


    class_<ModuleStore, boost::noncopyable>("ModuleStore")
    .def("Size", &ModuleStore::Size)
    .def("Has", &ModuleStore::Has)
    .def("RemoveModule", static_cast<void(ModuleStore::*)(unsigned long)>(&ModuleStore::RemoveModule))
    .def("SetOptions", Wrap_ModuleStore_SetOptions)
    .def("GetKeys", &ModuleStore::GetKeys)
    .def("KeyInfo", &ModuleStore::KeyInfo)
    .def("GetModule", &ModuleStore::GetModule<ModuleBase>, return_internal_reference<>())
    .def("GetModule_Test", &ModuleStore::GetModule<Test_Base>, return_internal_reference<>())
    .def("GetScopedModule", Wrap_GetScopedModule<ModuleBase>)
    .def("GetScopedModule_Test", Wrap_GetScopedModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleStore *>())
    .def("CloseHandles", &CModuleLoader::CloseHandles)
    .def("UnloadAll", &CModuleLoader::UnloadAll)
    .def("LoadSO", Wrap_CModuleLoader_LoadSO);

    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleStore *>())
    .def("UnloadAll", &PyModuleLoader::UnloadAll)
    .def("AddPyModule", Wrap_PyModuleLoader_AddPyModule);

}


} // close namespace export_python
} // close namespace modulestore
} // close namespace bpmodule

