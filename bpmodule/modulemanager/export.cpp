/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Pybind11_functional.hpp"

// Various components
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulemanager/CppModuleLoader.hpp"

using bpmodule::modulebase::ModuleBase;


namespace bpmodule {
namespace modulemanager {
namespace export_python {


PYBIND11_PLUGIN(modulemanager)
{
    pybind11::module m("modulemanager", "Module loading and location");

    //////////////////////
    // ModuleInfo
    //////////////////////
    pybind11::class_<ModuleInfo>(m, "ModuleInfo")
    .def(pybind11::init<>())
    .def("Print", &ModuleInfo::Print)
    .def_readwrite("name", &ModuleInfo::name)
    .def_readwrite("type", &ModuleInfo::type)
    .def_readwrite("path", &ModuleInfo::path)
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
    .def("LoadModuleFromModuleInfo", &ModuleManager::LoadModuleFromModuleInfo)
    .def("Size", &ModuleManager::Size)
    .def("AddKey", &ModuleManager::AddKey)
    .def("ReplaceKey", &ModuleManager::ReplaceKey)
    .def("ModuleKeyInfo", &ModuleManager::ModuleKeyInfo)
    .def("ModuleNameInfo", &ModuleManager::ModuleNameInfo)
    .def("PrintInfo", &ModuleManager::PrintInfo)
    .def("HasKey", &ModuleManager::HasKey)
    .def("HasName", &ModuleManager::HasName)
    .def("TestAll", &ModuleManager::TestAll)
    .def("GetModule", &ModuleManager::GetModulePy)
    .def("ChangeOption", &ModuleManager::ChangeOptionPy)
    .def("DotGraph", &ModuleManager::DotGraph)
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
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace modulemanager
} // close namespace bpmodule

