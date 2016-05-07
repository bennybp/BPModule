/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "pulsar/python/Pybind11_operators.hpp"
#include "pulsar/python/Pybind11_stl.hpp"

#include "pulsar/datastore/CacheData.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/datastore/OptionMap.hpp"


// This is needed for the member data of
// the wavefunction struct
PYBIND11_DECLARE_HOLDER_TYPE(T,std::shared_ptr<T>);

using pulsar::system::System;
using pulsar::datastore::Wavefunction;


namespace pulsar{
namespace datastore {
namespace export_python {


// in testing_export.cpp
void export_testing(pybind11::module & m);


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
    .def("Has", &OptionMap::Has)
    .def("HasKey", &OptionMap::HasKey)
    .def("Size", &OptionMap::Size)
    .def("IsDefault", &OptionMap::IsDefault)
    .def("IsSet", &OptionMap::IsSet)
    .def("ResetToDefault", &OptionMap::ResetToDefault)
    .def("AllReqSet", &OptionMap::AllReqSet)
    .def("Print", &OptionMap::Print)
    .def("AddOption", &OptionMap::AddOption)
    .def("Change", &OptionMap::ChangePy)
    .def("Get", &OptionMap::GetPy)
    .def("Compare", &OptionMap::Compare)
    .def("HasIssues", &OptionMap::HasIssues)
    .def("MyHash", &OptionMap::MyHash)
    .def("HashValues", &OptionMap::HashValues)
    .def("HashAllValues", &OptionMap::HashAllValues)
    .def(pybind11::self == pybind11::self)
    ;



    ///////////////////////
    // Wavefunction
    ///////////////////////
    pybind11::class_<Wavefunction>(m, "Wavefunction")
    .def(pybind11::init<>())
    .def(pybind11::self == pybind11::self)
    .def(pybind11::self != pybind11::self)
    .def("GetSystem", &Wavefunction::GetSystem)
    .def("GetSystemHash", &Wavefunction::GetSystemHash)
    .def("SetSystem", static_cast<void (Wavefunction::*)(std::shared_ptr<const system::System>)>(&Wavefunction::SetSystem))
    .def("GetCMat", &Wavefunction::GetCMat)
    .def("GetCMatHash", &Wavefunction::GetCMatHash)
    .def("SetCMat", static_cast<void (Wavefunction::*)(std::shared_ptr<const math::IrrepSpinMatrixD>)>(&Wavefunction::SetCMat))
    .def("GetEpsilon", &Wavefunction::GetEpsilon)
    .def("GetEpsilonHash", &Wavefunction::GetEpsilonHash)
    .def("SetEpsilon", static_cast<void (Wavefunction::*)(std::shared_ptr<const math::IrrepSpinVectorD>)>(&Wavefunction::SetEpsilon))
    .def("GetOccupations", &Wavefunction::GetOccupations)
    .def("GetOccupationsHash", &Wavefunction::GetOccupationsHash)
    .def("SetOccupations", static_cast<void (Wavefunction::*)(std::shared_ptr<const math::IrrepSpinVectorD>)>(&Wavefunction::SetOccupations))
    ;
  

    ////////////////////////////////////////
    // CacheData
    // Can just store python object
    //!\todo GetCopy and GetRef are equivalent for python
    ////////////////////////////////////////
    pybind11::class_<CacheData>(m, "CacheData")
    .def("Size", &CacheData::Size)
    .def("GetKeys", &CacheData::GetKeys)
    .def("Erase", &CacheData::Erase)
    .def("Print", &CacheData::Print)
    .def("Count", &CacheData::Count,
                   "See if the cache has some data",
                   pybind11::arg("key"))
    .def("Get", &CacheData::GetPy, 
                "Get reference", pybind11::return_value_policy::reference_internal,
                pybind11::arg("key"))
    .def("Set", &CacheData::SetPy, 
                "Set data",
                pybind11::arg("key"), 
                pybind11::arg("obj"))
    ;


    // Export the testing stuff
    export_testing(m);

    return m.ptr();
}


} // close namespace export_python
} // close namespace datastore
} // close namespace pulsar

