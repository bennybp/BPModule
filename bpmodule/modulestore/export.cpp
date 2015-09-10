/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


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
boost::shared_ptr<T> Wrap_GetModule(ModuleStore * ms, const std::string & key)
{
    T & mod = ms->GetModuleRef<T>(key);
    std::function<void(ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(ModuleBase *)>(&ModuleStore::RemoveModule), ms, std::placeholders::_1);
    return boost::shared_ptr<T>(&mod, dfunc);
}


// wraps CModuleLoader::LoadSO so that it can take a dict for the ModuleInfo
void Wrap_CModuleLoader_LoadSO(CModuleLoader * ml, const std::string & key, const boost::python::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->LoadSO(key, minfo);
}

// wraps PyModuleLoader::AddPyModule so that it can take a dict for the ModuleInfo
void Wrap_PyModuleLoader_AddPyModule(PyModuleLoader * ml,
                                     const std::string & key, boost::python::object func,
                                     const boost::python::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->AddPyModule(key, func, minfo);
}


void Wrap_ModuleStore_SetOptions(ModuleStore * mst, const std::string & key, boost::python::list & opt)
{
    // OptionMap has a conversion constructor
    mst->SetOptions(key, opt);
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
    .def("SetOptions", Wrap_ModuleStore_SetOptions)
    .def("GetKeys", &ModuleStore::GetKeys)
    .def("KeyInfo", &ModuleStore::KeyInfo)
    .def("GetModule", Wrap_GetModule<ModuleBase>)
    .def("GetModule_Test", Wrap_GetModule<Test_Base>);


    class_<CModuleLoader, boost::noncopyable>("CModuleLoader", init<ModuleStore *>())
    .def("LoadSO", Wrap_CModuleLoader_LoadSO)
    .def("CloseHandles", &CModuleLoader::CloseHandles)
    .def("UnloadAll", &CModuleLoader::UnloadAll);


    class_<PyModuleLoader, boost::noncopyable>("PyModuleLoader", init<ModuleStore *>())
    .def("UnloadAll", &PyModuleLoader::UnloadAll)
    .def("AddPyModule", Wrap_PyModuleLoader_AddPyModule);

}


} // close namespace export_python
} // close namespace modulestore
} // close namespace bpmodule

