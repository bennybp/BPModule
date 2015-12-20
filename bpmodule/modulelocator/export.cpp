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
    // Main ModuleLocator and
    // module loaders
    //////////////////////////
    pybind11::class_<ModuleLocator>(m, "ModuleLocator")
    .def(pybind11::init<>())
    .def("InsertModule", &ModuleLocator::InsertModule)
    .def("Size", &ModuleLocator::Size)
    .def("GetKeys", &ModuleLocator::GetKeys)
    .def("KeyInfo", &ModuleLocator::KeyInfo)
    .def("PrintInfo", &ModuleLocator::PrintInfo)
    .def("Has", &ModuleLocator::Has)
    .def("TestAll", &ModuleLocator::TestAll)
    .def("ClearCache", &ModuleLocator::ClearCache)
    .def("ClearStore", &ModuleLocator::ClearStore)
    .def("GetModule", &ModuleLocator::GetModulePy)
    .def("ChangeOption", &ModuleLocator::ChangeOptionPy)
    ;


    pybind11::class_<CppModuleLoader>(m, "CppModuleLoader")
    .def(pybind11::init<ModuleLocator *>())
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
    pybind11::class_<CreatorFunctions>(m, "CreatorFunctions")
    .def(pybind11::init<>())
    .def("AddPyCreator", &CreatorFunctions::AddPyCreator)
    .def("HasCreator", &CreatorFunctions::HasCreator)
    .def("GetCreator", &CreatorFunctions::GetCreator)
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace modulelocator
} // close namespace bpmodule

