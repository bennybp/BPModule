/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "pulsar/datastore/CacheData.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/datastore/OptionMap.hpp"


// This is needed for the member data of
// the wavefunction struct
PYBIND11_DECLARE_HOLDER_TYPE(_T_,std::shared_ptr<_T_>);

using pulsar::system::System;
using pulsar::datastore::Wavefunction;


namespace pulsar{
namespace datastore {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);


void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("datastore", "Data storage classes");


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
    .def("has", &OptionMap::has)
    .def("has_key", &OptionMap::has_key)
    .def("size", &OptionMap::size)
    .def("is_default", &OptionMap::is_default)
    .def("is_set", &OptionMap::is_set)
    .def("reset_to_default", &OptionMap::reset_to_default)
    .def("all_req_set", &OptionMap::all_req_set)
    .def("print", &OptionMap::print)
    .def("add_option", &OptionMap::add_option)
    .def("change", &OptionMap::change_py)
    .def("get", &OptionMap::get_py)
    .def("compare", &OptionMap::compare)
    .def("has_issues", &OptionMap::has_issues)
    .def("my_hash", &OptionMap::my_hash)
    .def("hash_values", &OptionMap::hash_values)
    .def("hash_all_values", &OptionMap::hash_all_values)
    .def(pybind11::self == pybind11::self)
    ;



    ///////////////////////
    // Wavefunction
    ///////////////////////
    pybind11::class_<Wavefunction>(m, "Wavefunction")
    .def(pybind11::init<>())
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("valid_check", &Wavefunction::valid_check)
    .def_readwrite("system", &Wavefunction::system)
    .def_readwrite("cmat", &Wavefunction::cmat)
    .def_readwrite("opdm", &Wavefunction::opdm)
    .def_readwrite("occupations", &Wavefunction::occupations)
    .def_readwrite("epsilon", &Wavefunction::epsilon)
    ;
  

    ////////////////////////////////////////
    // CacheData
    // Can just store python object
    ////////////////////////////////////////
    pybind11::class_<CacheData>(m, "CacheData")
    .def("size", &CacheData::size)
    .def("get_keys", &CacheData::get_keys)
    .def("erase", &CacheData::erase)
    .def("print", &CacheData::print)
    .def("count", &CacheData::count,
                   "See if the cache has some data",
                   pybind11::arg("key"))
    .def("get", &CacheData::get_py, 
                "Get reference", pybind11::return_value_policy::reference_internal,
                pybind11::arg("key"))
    .def("set", &CacheData::set_py, 
                "Set data",
                pybind11::arg("key"), 
                pybind11::arg("obj"))
    ;


    // Export the testing stuff
    export_testing(m);
}


} // close namespace export_python
} // close namespace datastore
} // close namespace pulsar

