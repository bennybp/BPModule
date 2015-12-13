/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/datastore/OptionMap.hpp"



using bpmodule::basisset::BasisSet;
using bpmodule::molecule::Molecule;
using bpmodule::datastore::Wavefunction;
using bpmodule::tensor::DistMatrixD;


namespace bpmodule {
namespace datastore {
namespace export_python {



PYBIND11_PLUGIN(datastore)
{
    pybind11::module m("datastore", "Data storage classes");

    /////////////////////////
    // OptionMap
    /////////////////////////
    // OptionMap always returns copies, so this should be safe
    pybind11::class_<OptionMap>(m, "OptionMap")
    .def(pybind11::init<const std::string &, pybind11::dict, pybind11::object>())
    .def("MaxInt", &OptionMap::MaxInt)
    .def("MinInt", &OptionMap::MinInt)
    .def("MaxFloat", &OptionMap::MaxFloat)
    .def("MinFloat", &OptionMap::MinFloat)
    .def("Get", &OptionMap::GetPy)
    .def("Has", &OptionMap::Has)
    .def("HasKey", &OptionMap::HasKey)
    .def("Size", &OptionMap::Size)
    .def("ResetToDefault", &OptionMap::ResetToDefault)
    .def("AllReqSet", &OptionMap::AllReqSet)
    .def("Print", &OptionMap::Print)
    .def("Change", &OptionMap::ChangePy)
    .def("ChangeDict", &OptionMap::ChangePyDict)
    .def("LockValid", &OptionMap::LockValid)
    .def("Validate", &OptionMap::Validate)
    .def("ModuleKey", &OptionMap::ModuleKey, pybind11::return_value_policy::copy)
    .def("Compare", &OptionMap::Compare)
    .def("CompareSelect", &OptionMap::CompareSelectPy)
    .def("HasIssues", &OptionMap::HasIssues);


    ///////////////////////
    // Wavefunction
    ///////////////////////
    pybind11::class_<Wavefunction>(m, "Wavefunction") //! \todo python init for wfn?
    .def("UniqueString", &Wavefunction::UniqueString)
    .def_readwrite("basis", &Wavefunction::basis)
    .def_readwrite("molecule", &Wavefunction::molecule)
    .def_readwrite("cmat", &Wavefunction::cmat)
    .def_readwrite("epsilon", &Wavefunction::epsilon)
    ;
  

    m.def("MakeUIDPointer", MakeUIDPointerPy);



    ////////////////////////////////////////
    // CacheData
    // Can just store python object
    // //! \todo GetCopy and GetRef are equivalent for python
    ////////////////////////////////////////
    pybind11::class_<CacheData>(m, "CacheData")
    .def("CountKey", &CacheData::CountKey)
    .def("Size", &CacheData::Size)
    .def("GetKeys", &CacheData::GetKeys)
    .def("Erase", &CacheData::Erase)
    .def("HasKey", &CacheData::HasKeyPy)
    .def("GetCopy", &CacheData::GetCopyPy, pybind11::return_value_policy::copy)
    .def("GetRef", &CacheData::GetRefPy, pybind11::return_value_policy::reference_internal)
    .def("Set", &CacheData::SetPy, pybind11::return_value_policy::reference_internal)
    ;
    /*
    .def("HasKey", &CacheData::HasKeyPy,
                   "See if the cache has some data",
                   pybind11::arg("key"),
                   pybind11::arg("opt") = OptionMap(),
                   pybind11::arg("sigopt") = pybind11::list())
    .def("GetCopy", &CacheData::GetCopyPy,
                   "Get a copy of the data", pybind11::return_value_policy::copy,
                   pybind11::arg("key"),
                   pybind11::arg("opt") = OptionMap(),
                   pybind11::arg("sigopt") = pybind11::list())
                   
    .def("GetRef", &CacheData::GetRefPy, 
                   "Get reference", pybind11::return_value_policy::reference_internal,
                   pybind11::arg("key"),
                   pybind11::arg("opt") = OptionMap(),
                   pybind11::arg("sigopt") = pybind11::list())
    .def("Set", static_cast<void(CacheData::*)(const std::string &, const pybind11::object &, const OptionMap &)>(&CacheData::Set),
                "Set data",
                pybind11::arg("key"), 
                pybind11::arg("obj"),
                pybind11::arg("opt") = OptionMap())
    ;
    */

    return m.ptr();
}


} // close namespace export_python
} // close namespace datastore
} // close namespace bpmodule

