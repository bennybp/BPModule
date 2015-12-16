/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Pybind11_functional.hpp"

// Various components
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulelocator/CModuleLoader.hpp"

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
    .def(pybind11::init<pybind11::dict>())
    .def("Print", &ModuleInfo::Print)
    .def_readonly("key", &ModuleInfo::key)
    .def_readonly("name", &ModuleInfo::name)
    .def_readonly("type", &ModuleInfo::type)
    .def_readonly("path", &ModuleInfo::path)
    .def_readonly("soname", &ModuleInfo::soname)
    .def_readonly("version", &ModuleInfo::version)
    .def_readonly("description", &ModuleInfo::description)
    .def_readonly("authors", &ModuleInfo::authors)
    .def_readonly("refs", &ModuleInfo::refs)
    .def_readonly("options", &ModuleInfo::options)
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
    .def("SetOptions", static_cast<void(ModuleLocator::*)(const std::string &, pybind11::dict)>(&ModuleLocator::SetOptions))
    .def("TestAll", &ModuleLocator::TestAll)
    .def("ClearCache", &ModuleLocator::ClearCache)
    .def("ClearStore", &ModuleLocator::ClearStore)
    .def("GetModule", &ModuleLocator::GetModulePy)
    ;


    pybind11::class_<CModuleLoader>(m, "CModuleLoader")
    .def(pybind11::init<ModuleLocator *>())
    .def("LoadSO", static_cast<void(CModuleLoader::*)(const std::string &, pybind11::dict)>(&CModuleLoader::LoadSO));


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

