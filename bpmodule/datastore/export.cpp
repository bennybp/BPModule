/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/copy_const_reference.hpp>

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

BOOST_PYTHON_MODULE(datastore)
{
    /* Similar to CalcData, python will have to work with copies
     */
    class_<Wavefunction>("Wavefunction", no_init) //! \todo python init for wfn?
    .def("Basis", &Wavefunction::Basis, return_value_policy<copy_const_reference>()) 
    .def("Molecule", &Wavefunction::Molecule, return_value_policy<copy_const_reference>()) 
    .def("CMat", &Wavefunction::CMat, return_value_policy<copy_const_reference>()) 
    .def("Epsilon", &Wavefunction::Epsilon, return_value_policy<copy_const_reference>()) 
    ;
  

    /*
     * GetRef() is not exported to python. This is because python does not enforce
     * const semantics, which could lead to behavior where the underlying data
     * is changed. This goes against the "immutability" of the PropertyMap and CalcData.
     * Python, unfortunately, would have to work with copies.
     */
    /////////////////////////
    // CalcData
    /////////////////////////
    class_<CalcData>("CalcData", init<>())
    .def(init<const CalcData &>())
    .def("HasKey", &CalcData::HasKey)
    .def("GetCopy", &CalcData::GetCopyPy)
    .def("Set", &CalcData::SetPy)
    .def("Erase", &CalcData::Erase)
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &, const std::string &)>(&CalcData::SetRef))
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &)>(&CalcData::SetRef));
}


} // close namespace export_python
} // close namespace datastore
} // close namespace bpmodule

