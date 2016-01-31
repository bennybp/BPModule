/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Pybind11_functional.hpp"

// Various components
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulelocator/CppModuleLoader.hpp"

using bpmodule::modulebase::ModuleBase;


namespace bpmodule {
namespace modulelocator {
namespace export_python {


PYBIND11_PLUGIN(modulelocator)
{
    pybind11::module m("modulelocator", "Module loading and location");

    //////////////////////
    // ModuleInfo
    //////////////////////
    pybind11::class_<ModuleInfo>(m, "ModuleInfo")
    .def(pybind11::init<>())
    .def("Print", &ModuleInfo::Print)
    .def_readwrite("key", &ModuleInfo::key)
    .def_readwrite("name", &ModuleInfo::name)
    .def_readwrite("type", &ModuleInfo::type)
    .def_readwrite("path", &ModuleInfo::path)
    .def_readwrite("soname", &ModuleInfo::soname)
    .def_readwrite("version", &ModuleInfo::version)
    .def_readwrite("description", &ModuleInfo::description)
    .def_readwrite("authors", &ModuleInfo::authors)
    .def_readwrite("refs", &ModuleInfo::refs)
    .def_readwrite("options", &ModuleInfo::options)
    ;
     


    //////////////////////////
    // Main ModuleManager and
    // module loaders
    //////////////////////////
    pybind11::class_<ModuleManager>(m, "ModuleManager")
    .def(pybind11::init<>())
    .def("InsertModule", &ModuleManager::InsertModule)
    .def("Size", &ModuleManager::Size)
    .def("GetModuleKeys", &ModuleManager::GetModuleKeys)
    .def("ModuleKeyInfo", &ModuleManager::ModuleKeyInfo)
    .def("PrintInfo", &ModuleManager::PrintInfo)
    .def("Has", &ModuleManager::Has)
    .def("TestAll", &ModuleManager::TestAll)
    .def("ClearCache", &ModuleManager::ClearCache)
    .def("ClearStore", &ModuleManager::ClearStore)
    .def("GetModule", &ModuleManager::GetModulePy)
    .def("ChangeOption", &ModuleManager::ChangeOptionPy)
    .def("DotGraph", &ModuleManager::DotGraph)
    ;


    pybind11::class_<CppModuleLoader>(m, "CppModuleLoader")
    .def(pybind11::init<ModuleManager *>())
    .def("LoadSO", &CppModuleLoader::LoadSO)
    ;


    ////////////////////////////////
    // Pointers, etc
    ////////////////////////////////
    pybind11::class_<PyModulePtr>(m, "ModulePtr")
    .def("__getattr__", &PyModulePtr::Py__getattr__)
    ;



    ///////////////////////////////
    // Creation functions
    ///////////////////////////////
    pybind11::class_<ModuleCreationFuncs>(m, "ModuleCreationFuncs")
    .def(pybind11::init<>())
    .def("AddPyCreator", &ModuleCreationFuncs::AddPyCreator)
    .def("HasCreator", &ModuleCreationFuncs::HasCreator)
    .def("GetCreator", &ModuleCreationFuncs::GetCreator)
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace modulelocator
} // close namespace bpmodule

