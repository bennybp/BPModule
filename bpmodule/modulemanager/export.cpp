/*! \file
 *
 * \brief Python exports for module storage and database
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/modulemanager/ModuleManager.hpp"


namespace bpmodule {
namespace modulemanager {
namespace export_python {

std::string DumpTree(const ModuleManager & mm, unsigned long startid)
{
    std::stringstream ss;

    for(auto it  = mm.TreeBegin(startid);
        it != mm.TreeEnd();
        ++it)
    {
        ss << "|" << it->moduleid << "| "
           << it->minfo.name << " : (" << it->parentid << ") [ ";
        for(const auto & c : it->children)
            ss << c << " ";
        ss << "]\n";
    }
    return ss.str();
}


std::string DumpFlatTree(const ModuleManager & mm)
{
    std::stringstream ss;

    for(auto it  = mm.FlatTreeBegin();
        it != mm.FlatTreeEnd();
        ++it)
    {
        ss << "|" << it->moduleid << "| "
           << it->minfo.name << " : (" << it->parentid << ") [ ";
        for(const auto & c : it->children)
            ss << c << " ";
        ss << "]\n";
    }
    return ss.str();
}


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
    ;

    m.def("DumpTree", DumpTree);
    m.def("DumpFlatTree", DumpFlatTree);

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

