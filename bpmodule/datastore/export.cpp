/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/return_internal_reference.hpp>

#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"



using namespace boost::python;
using bpmodule::basisset::BasisSet;
using bpmodule::molecule::Molecule;
using bpmodule::datastore::Wavefunction;
using bpmodule::tensor::DistMatrixD;


namespace bpmodule {
namespace datastore {
namespace export_python {


// These macros generate some compiler warnings
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_UNUSED_LOCAL_TYPEDEFS
PRAGMA_WARNING_IGNORE_SHADOW

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CacheData_HasKeyPy_Overloads, HasKeyPy, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CacheData_GetCopyPy_Overloads, GetCopyPy, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CacheData_GetRefPy_Overloads, GetRefPy, 1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CacheData_Set_Overloads, Set, 2, 3)

PRAGMA_WARNING_POP



BOOST_PYTHON_MODULE(datastore)
{
    class_<Wavefunction>("Wavefunction", no_init) //! \todo python init for wfn?
    .def("UniqueString", &Wavefunction::UniqueString)
    .def_readwrite("basis", &Wavefunction::basis)
    .def_readwrite("molecule", &Wavefunction::molecule)
    .def_readwrite("cmat", &Wavefunction::cmat)
    .def_readwrite("epsilon", &Wavefunction::epsilon)
    ;
  

    def("MakeUIDPointer", MakeUIDPointerPy);

    /*
    ////////////////////////////////////////
    // CalcData
    // Can just store boost::python::object
    ////////////////////////////////////////
    class_<CalcData>("CalcData", init<>())
    .def(init<const CalcData &>())
    .def("Size", &CalcData::Size)
    .def("GetKeys", &CalcData::GetKeys)
    .def("HasKey", &CalcData::HasKey)
    .def("Erase", &CalcData::Erase)
    .def("GetType", &CalcData::GetType)
    .def("GetDemangledType", &CalcData::GetDemangledType)
    .def("GetCopy", static_cast<boost::python::object(CalcData::*)(const std::string &) const>(&CalcData::GetCopy))
    .def("GetRef", static_cast<const boost::python::object &(CalcData::*)(const std::string &) const>(&CalcData::GetRef), return_internal_reference<>()) // copy it?
    .def("Set", static_cast<void(CalcData::*)(const std::string &, const boost::python::object &)>(&CalcData::Set))
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &, const std::string &)>(&CalcData::SetRef))
    ;
    */

    
    ////////////////////////////////////////
    // CacheData
    // Can just store boost::python::object
    ////////////////////////////////////////
    class_<CacheData>("CacheData", no_init)
    .def("CountKey", &CacheData::CountKey)
    .def("Size", &CacheData::Size)
    .def("GetKeys", &CacheData::GetKeys)
    .def("Erase", &CacheData::Erase)
    .def("HasKey", &CacheData::HasKeyPy, CacheData_HasKeyPy_Overloads())
    .def("GetCopy", &CacheData::GetCopyPy, CacheData_GetCopyPy_Overloads())
    .def("GetRef", &CacheData::GetRefPy, return_internal_reference<>(), CacheData_GetRefPy_Overloads())
    .def("Set", static_cast<void(CacheData::*)(const std::string &, const boost::python::object &, const options::OptionMap &)>(&CacheData::Set), CacheData_Set_Overloads())
    ;

}


} // close namespace export_python
} // close namespace datastore
} // close namespace bpmodule

