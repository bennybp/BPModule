/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

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
    // for now, we won't allow initialization from python
    pybind11::class_<ModuleInfo>(m, "ModuleInfo")
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
    .def("Size", &ModuleLocator::Size)
    .def("GetKeys", &ModuleLocator::GetKeys)
    .def("KeyInfo", &ModuleLocator::KeyInfo)
    .def("PrintInfo", &ModuleLocator::PrintInfo)
    .def("Has", &ModuleLocator::Has)
    .def("SetOptions", static_cast<void(ModuleLocator::*)(const std::string &, pybind11::dict)>(&ModuleLocator::SetOptions))
    .def("TestAll", &ModuleLocator::TestAll)
    .def("ClearCache", &ModuleLocator::ClearCache)
    .def("GetModule", &ModuleLocator::GetModulePy)
    ;


    pybind11::class_<CModuleLoader>(m, "CModuleLoader")
    .def(pybind11::init<ModuleLocator *>())
    .def("LoadSO", static_cast<void(CModuleLoader::*)(const std::string &, pybind11::dict)>(&CModuleLoader::LoadSO));

/*
    pybind11::class_<PyModuleLoader>(m, "PyModuleLoader")
    .def(pybind11::init<ModuleLocator *>())
    .def("LoadPyModule", static_cast<void(PyModuleLoader::*)(const std::string &, pybind11::object, pybind11::dict)>(&PyModuleLoader::LoadPyModule));
*/

    return m.ptr();
}


} // close namespace export_python
} // close namespace modulelocator
} // close namespace bpmodule

