/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"

#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/datastore/OptionMap.hpp"


// This is needed for the member data of
// the wavefunction struct
PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);

using bpmodule::system::System;
using bpmodule::datastore::Wavefunction;


namespace bpmodule {
namespace datastore {
namespace export_python {




PYBIND11_PLUGIN(datastore)
{
    pybind11::module m("datastore", "Data storage classes");


    //////////////////
    // OptionTypes
    //////////////////
    pybind11::enum_<OptionType>(m, "OptionType")
    .value("Int", OptionType::Int)
    .value("Float", OptionType::Float)
    .value("Bool", OptionType::Bool)
    .value("String", OptionType::String)
    .value("ListInt", OptionType::ListInt)
    .value("ListFloat", OptionType::ListFloat)
    .value("ListBool", OptionType::ListBool)
    .value("ListString", OptionType::ListString)
    .value("SetInt", OptionType::SetInt)
    .value("SetFloat", OptionType::SetFloat)
    .value("SetBool", OptionType::SetBool)
    .value("SetString", OptionType::SetString)

    .value("DictIntInt", OptionType::DictIntInt)
    .value("DictIntFloat", OptionType::DictIntFloat)
    .value("DictIntBool", OptionType::DictIntBool)
    .value("DictIntString", OptionType::DictIntString)
    .value("DictStringInt", OptionType::DictStringInt)
    .value("DictStringFloat", OptionType::DictStringFloat)
    .value("DictStringBool", OptionType::DictStringBool)
    .value("DictStringString", OptionType::DictStringString)
    ;


    /////////////////////////
    // OptionMap
    /////////////////////////
    // OptionMap always returns copies
    pybind11::class_<OptionMap>(m, "OptionMap")
    .def(pybind11::init<>())
    .def(pybind11::init<const std::string &>())
    .def("Has", &OptionMap::Has)
    .def("HasKey", &OptionMap::HasKey)
    .def("Size", &OptionMap::Size)
    .def("ResetToDefault", &OptionMap::ResetToDefault)
    .def("AllReqSet", &OptionMap::AllReqSet)
    .def("Print", &OptionMap::Print)
    .def("AddOption", &OptionMap::AddOption)
    .def("Change", &OptionMap::ChangePy)
    .def("Get", &OptionMap::GetPy)
    .def("LockValid", &OptionMap::LockValid)
    .def("Validate", &OptionMap::Validate)
    .def("ModuleKey", &OptionMap::ModuleKey, pybind11::return_value_policy::copy)
    .def("Compare", &OptionMap::Compare)
    .def("CompareSelect", &OptionMap::CompareSelect)
    .def("HasIssues", &OptionMap::HasIssues);



    ///////////////////////
    // Wavefunction
    ///////////////////////
    pybind11::class_<Wavefunction>(m, "Wavefunction") //! \todo python init for wfn?
    .def(pybind11::init<>())
    .def("UniqueString", &Wavefunction::UniqueString)
    .def_readwrite("system", &Wavefunction::system)
    ;
  

    ////////////////////////////////////////
    // CacheData
    // Can just store python object
    //!\todo GetCopy and GetRef are equivalent for python
    ////////////////////////////////////////
    pybind11::class_<CacheData>(m, "CacheData")
    .def("CountKey", &CacheData::CountKey)
    .def("Size", &CacheData::Size)
    .def("GetKeys", &CacheData::GetKeys)
    .def("Erase", &CacheData::Erase)
    .def("Print", &CacheData::Print)
    .def("HasData", &CacheData::HasData,
                   "See if the cache has some data",
                   pybind11::arg("key"),
                   pybind11::arg("opt") = OptionMap(),
                   pybind11::arg("sigopt") = pybind11::list())
    .def("Get", &CacheData::GetPy, 
                "Get reference", pybind11::return_value_policy::reference_internal,
                pybind11::arg("key"),
                pybind11::arg("opt") = OptionMap(),
                pybind11::arg("sigopt") = pybind11::list())
    .def("Set", &CacheData::SetPy, 
                "Set data",
                pybind11::arg("key"), 
                pybind11::arg("obj"),
                pybind11::arg("opt") = OptionMap())
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace datastore
} // close namespace bpmodule

