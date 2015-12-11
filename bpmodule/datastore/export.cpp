/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/return_internal_reference.hpp>

#include "bpmodule/datastore/CalcData.hpp"
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"



using namespace boost::python;
using bpmodule::basisset::BasisSet;
using bpmodule::molecule::Molecule;
using bpmodule::datastore::Wavefunction;
using bpmodule::tensor::DistMatrixD;


namespace bpmodule {
namespace datastore {
namespace export_python {

template<typename T>
static void
RegisterUIDPointer(void)
{
    std::string pyname = std::string("UIDPointer_") + typeid(T).name();

    class_<UIDPointer<T>>(pyname.c_str(), no_init) //! \todo python init for UIDPointer?
    .def("Valid", &UIDPointer<T>::Valid)
    .def("UID", &UIDPointer<T>::UID)
    .def("Set", &UIDPointer<T>::Set)
    .def("Get", &UIDPointer<T>::Get, return_internal_reference<>())
    ;
}

BOOST_PYTHON_MODULE(datastore)
{
    RegisterUIDPointer<BasisSet>();
    RegisterUIDPointer<Molecule>();
    RegisterUIDPointer<DistMatrixD>();

    /* Similar to CalcData, python will have to work with copies
     */
    class_<Wavefunction>("Wavefunction", no_init) //! \todo python init for wfn?
    .def("UniqueString", &Wavefunction::UniqueString)
    .def_readwrite("basis", &Wavefunction::basis)
    .def_readwrite("molecule", &Wavefunction::molecule)
    .def_readwrite("cmat", &Wavefunction::cmat)
    .def_readwrite("epsilon", &Wavefunction::epsilon)
    ;
  

    /*
     * GetRef() is not exported to python. This is because python does not enforce
     * const semantics, which could lead to behavior where the underlying data
     * is changed. This goes against the "immutability" of the PropertyMap and CalcData.
     */
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
}


} // close namespace export_python
} // close namespace datastore
} // close namespace bpmodule

