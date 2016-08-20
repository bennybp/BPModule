/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/modulemanager/CheckpointIO.hpp"

#include "pulsar/modulemanager/checkpoint_backends/DummyCheckpointIO.hpp"
#include "pulsar/modulemanager/checkpoint_backends/BDBCheckpointIO.hpp"

PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);

using pulsar::modulemanager::detail::ConstModuleTreeIter;
using pulsar::modulemanager::detail::ConstModuleFlatTreeIter;

namespace pulsar{
namespace modulemanager {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);

void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("modulemanager", "Module loading and location");

    //////////////////////
    // ModuleInfo
    //////////////////////
    pybind11::class_<ModuleInfo>(m, "ModuleInfo")
    .def(pybind11::init<>())
    .def("print", &ModuleInfo::print)
    .def("my_hash", &ModuleInfo::my_hash)
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
    .def_readonly("parentid", &ModuleTreeNode::parentid)
    .def_readonly("children", &ModuleTreeNode::children)
    ;


    //////////////////////////
    // Tree iterators
    //////////////////////////
    pybind11::class_<ConstModuleTreeIter>(m, "ConstModuleTreeIter")
    .def("advance", &ConstModuleTreeIter::advance)
    .def("get_ref", &ConstModuleTreeIter::get_ref, pybind11::return_value_policy::reference_internal)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;

    pybind11::class_<ConstModuleFlatTreeIter>(m, "ConstModuleFlatTreeIter")
    .def("advance", &ConstModuleFlatTreeIter::advance)
    .def("get_ref", &ConstModuleFlatTreeIter::get_ref, pybind11::return_value_policy::reference_internal)
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    ;


    //////////////////////////
    // Main ModuleManager
    //////////////////////////
    pybind11::class_<ModuleManager>(m, "ModuleManager")
    .def(pybind11::init<>())
    .def("size", &ModuleManager::size)
    .def("module_key_info", &ModuleManager::module_key_info)
    .def("print", &ModuleManager::print)
    .def("has_key", &ModuleManager::has_key)
    .def("duplicate_key", &ModuleManager::duplicate_key)
    .def("generate_unique_key",&ModuleManager::generate_unique_key)
    .def("test_all", &ModuleManager::test_all)
    .def("get_module", &ModuleManager::get_module_py)
    .def("module_in_use", &ModuleManager::module_in_use)
    .def("change_option", &ModuleManager::change_option_py)
    .def("load_module_from_minfo", &ModuleManager::load_module_from_minfo)
    .def("enable_debug", &ModuleManager::enable_debug)
    .def("enable_debug_all", &ModuleManager::enable_debug_all)
    ;


    ////////////////////////////////
    // Pointers, etc
    ////////////////////////////////
    pybind11::class_<PyModulePtr>(m, "ModulePtr")
    .def("__getattr__", &PyModulePtr::py__getattr__)
    ;


    ///////////////////////////////
    // Creation functions
    ///////////////////////////////
    pybind11::class_<ModuleCreationFuncs>(m, "ModuleCreationFuncs")
    .def(pybind11::init<>())
    .def("add_py_creator", &ModuleCreationFuncs::add_py_creator)
    .def("has_creator", &ModuleCreationFuncs::has_creator)
    ;


    //////////////////////////////
    // Checkpointing Backends
    //////////////////////////////

    // No need to export most of the member functions of these classes.
    // Those are meant to be used only from C++ (for now)
    pybind11::class_<CheckpointIO, std::shared_ptr<CheckpointIO>> cpio(m, "CheckpointIO");

    pybind11::class_<DummyCheckpointIO, std::shared_ptr<DummyCheckpointIO>>(m, "DummyCheckpointIO", cpio)
    .def(pybind11::init<>())
    ;

    pybind11::class_<BDBCheckpointIO, std::shared_ptr<BDBCheckpointIO>>(m, "BDBCheckpointIO", cpio)
    .def(pybind11::init<const std::string &, bool>())
    ;

    //////////////////////////////
    // Checkpointing
    //////////////////////////////
    pybind11::class_<Checkpoint>(m, "Checkpoint")
    .def(pybind11::init<const std::shared_ptr<CheckpointIO> &, const std::shared_ptr<CheckpointIO> &>())
    .def("save_local_cache", &Checkpoint::save_local_cache)
    .def("load_local_cache", &Checkpoint::load_local_cache)
    .def("local_keys", &Checkpoint::local_keys)
    ;

    // Export the testing stuff
    export_testing(m);
}


} // close namespace export_python
} // close namespace modulemanager
} // close namespace pulsar

