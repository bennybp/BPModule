/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"

#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/datastore/ModuleGraph.hpp"


//! \todo pybind11 needs this for the default arguments
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_UNUSED_PARAMETERS
namespace pybind11 {
namespace detail {

    template<>
    std::string to_string(const bpmodule::datastore::OptionMap & opt)
    {
        return "OptionMap()";
    }
}
}
PRAGMA_WARNING_POP



using bpmodule::basisset::BasisSet;
using bpmodule::molecule::Molecule;
using bpmodule::datastore::Wavefunction;
using bpmodule::tensor::DistMatrixD;


namespace bpmodule {
namespace datastore {
namespace export_python {


template<OptionType OPTTYPE>
static void RegisterOptionHolder(pybind11::module & m, pybind11::class_<OptionBase> ohbase)
{
    const char * otname = OptionTypeToString(OPTTYPE);

    const std::string pyname = std::string("OptionHolder_") + otname;
    pybind11::class_<OptionHolder<OPTTYPE>>(m, pyname.c_str(), ohbase)
    .def(pybind11::init<const std::string &, bool, pybind11::object, const std::string &>()) 
    .def(pybind11::init<const std::string &, bool, pybind11::object, const std::string &, const pybind11::object &>()) 
    ;
}

std::string Graph_PrintDot_Wrap(const ModuleGraph & g)
{
    std::stringstream ss;
    ss << g;
    return ss.str();
}




PYBIND11_PLUGIN(datastore)
{
    pybind11::module m("datastore", "Data storage classes");


    //////////////////
    // UIDPointer
    //////////////////
    m.def("MakeUIDPointer", MakeUIDPointerPy);



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
    ;


    /////////////////////////
    // OptionHolders
    /////////////////////////
    pybind11::class_<OptionBase> ohbase(m, "OptionBase");
    
    RegisterOptionHolder<OptionType::Int>(m, ohbase);
    RegisterOptionHolder<OptionType::Float>(m, ohbase);
    RegisterOptionHolder<OptionType::Bool>(m, ohbase);
    RegisterOptionHolder<OptionType::String>(m, ohbase);
    RegisterOptionHolder<OptionType::ListInt>(m, ohbase);
    RegisterOptionHolder<OptionType::ListFloat>(m, ohbase);
    RegisterOptionHolder<OptionType::ListBool>(m, ohbase);
    RegisterOptionHolder<OptionType::ListString>(m, ohbase);
    RegisterOptionHolder<OptionType::SetInt>(m, ohbase);
    RegisterOptionHolder<OptionType::SetFloat>(m, ohbase);
    RegisterOptionHolder<OptionType::SetBool>(m, ohbase);
    RegisterOptionHolder<OptionType::SetString>(m, ohbase);


    /////////////////////////
    // OptionMap
    /////////////////////////
    // OptionMap always returns copies
    pybind11::class_<OptionMap>(m, "OptionMap")
    .def(pybind11::init<>())
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
    .def_readwrite("basis", &Wavefunction::basis)
    .def_readwrite("molecule", &Wavefunction::molecule)
    .def_readwrite("cmat", &Wavefunction::cmat)
    .def_readwrite("epsilon", &Wavefunction::epsilon)
    ;
  

    ////////////////////////////////////////
    // Graph
    ////////////////////////////////////////
    pybind11::class_<ModuleGraph>(m, "ModuleGraph")
    .def("NNodes", &ModuleGraph::NNodes)
    .def("NEdges", static_cast<size_t (ModuleGraph::*)(void) const>(&ModuleGraph::NEdges))
    ;

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

