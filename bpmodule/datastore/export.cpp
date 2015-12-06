/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>

#include "bpmodule/datastore/CalcData.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"



using namespace boost::python;
using bpmodule::basisset::BasisSet;
using bpmodule::molecule::Molecule;
using bpmodule::datastore::Wavefunction;
using bpmodule::tensor::DistMatrixD;


//! \todo move to Wavefunction structure?
static BasisSet GetWfnBasis(const Wavefunction * wfn) { return *(wfn->basis); }
static void SetWfnBasis(Wavefunction * wfn, const BasisSet & basis) { wfn->basis = std::shared_ptr<const BasisSet>(new BasisSet(basis)); }

static Molecule GetWfnMolecule(const Wavefunction * wfn) { return *(wfn->molecule); }
static void SetWfnMolecule(Wavefunction * wfn, const Molecule & molecule) { wfn->molecule = std::shared_ptr<const Molecule>(new Molecule(molecule)); }

static DistMatrixD GetWfnCMat(const Wavefunction * wfn) { return *(wfn->cmat); }
static void SetWfnCMat(Wavefunction * wfn, const DistMatrixD & cmat) { wfn->cmat = std::shared_ptr<const DistMatrixD>(new DistMatrixD(cmat)); }




namespace bpmodule {
namespace datastore {
namespace export_python {

BOOST_PYTHON_MODULE(datastore)
{
    /* Similar to CalcData, python will have to work with copies
     */
    class_<Wavefunction>("Wavefunction", init<>()) //! \todo python init for wfn?
    .add_property("basis", GetWfnBasis, SetWfnBasis)
    .add_property("molecule", GetWfnMolecule, SetWfnMolecule)
    .add_property("cmat", GetWfnCMat, SetWfnCMat)
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
    .def("GetSource", &CalcData::GetSource)
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

