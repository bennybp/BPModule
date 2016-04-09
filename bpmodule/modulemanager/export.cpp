/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Pybind11_operators.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"

using bpmodule::modulemanager::detail::ConstModuleTreeIter;
using bpmodule::modulemanager::detail::ConstModuleFlatTreeIter;

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
    .def_readwrite("base", &ModuleInfo::base)
    .def_readwrite("path", &ModuleInfo::path)
    .def_readwrite("version", &ModuleInfo::version)
    .def_readwrite("description", &ModuleInfo::description)
    .def_readwrite("authors", &ModuleInfo::authors)
    .def_readwrite("refs", &ModuleInfo::refs)
    .def_readwrite("options", &ModuleInfo::options)
    ;

    //////////////////////////
    // Tree Node
    //////////////////////////     
    pybind11::class_<ModuleTreeNode>(m, "ModuleTreeNode")
    .def_readonly("modulekey", &ModuleTreeNode::modulekey)
    .def_readonly("minfo", &ModuleTreeNode::minfo)
    .def_readonly("output", &ModuleTreeNode::output)
    .def_readonly("id", &ModuleTreeNode::id)
    .def_readonly("initial_wfn", &ModuleTreeNode::initial_wfn)
    .def_readonly("final_wfn", &ModuleTreeNode::final_wfn)
    .def_readonly("parentid", &ModuleTreeNode::parentid)
    .def_readonly("children", &ModuleTreeNode::children)
    ;


    //////////////////////////
    // Tree iterators
    //////////////////////////     
    pybind11::class_<ConstModuleTreeIter>(m, "ConstModuleTreeIter")
    .def("Advance", &ConstModuleTreeIter::Advance)
    .def("GetRef", &ConstModuleTreeIter::GetRef, pybind11::return_value_policy::reference_internal)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;

    pybind11::class_<ConstModuleFlatTreeIter>(m, "ConstModuleFlatTreeIter")
    .def("Advance", &ConstModuleFlatTreeIter::Advance)
    .def("GetRef", &ConstModuleFlatTreeIter::GetRef, pybind11::return_value_policy::reference_internal)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;


    //////////////////////////
    // Main ModuleManager
    //////////////////////////
    pybind11::class_<ModuleManager>(m, "ModuleManager")
    .def(pybind11::init<>())
    .def("Size", &ModuleManager::Size)
    .def("ModuleKeyInfo", &ModuleManager::ModuleKeyInfo)
    .def("ModuleNameInfo", &ModuleManager::ModuleNameInfo)
    .def("Print", &ModuleManager::Print)
    .def("HasKey", &ModuleManager::HasKey)
    .def("HasName", &ModuleManager::HasName)
    .def("AddKey", &ModuleManager::AddKey)
    .def("ReplaceKey", &ModuleManager::ReplaceKey)
    .def("TestAll", &ModuleManager::TestAll)
    .def("GetModule", &ModuleManager::GetModulePy)
    .def("ChangeOption", &ModuleManager::ChangeOptionPy)
    .def("LoadModuleFromModuleInfo", &ModuleManager::LoadModuleFromModuleInfo)
    .def("EnableDebug", &ModuleManager::EnableDebug)
    .def("EnableDebugAll", &ModuleManager::EnableDebugAll)
    .def("TreeBegin", &ModuleManager::TreeBegin)
    .def("TreeEnd", &ModuleManager::TreeEnd)
    .def("FlatTreeBegin", &ModuleManager::FlatTreeBegin)
    .def("FlatTreeEnd", &ModuleManager::FlatTreeEnd)
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

