/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/datastore/CalcData.hpp"

#include <boost/python.hpp>

using namespace boost::python;


namespace bpmodule {
namespace datastore {
namespace export_python {

BOOST_PYTHON_MODULE(datastore)
{
    /////////////////////////
    // OptionMap
    /////////////////////////
    // OptionMap always returns copies, so this should be safe
    //! \todo HasType
    class_<OptionMap>("OptionMap", init<>())
    .def("Get", &OptionMap::GetPy)
    .def("Has", &OptionMap::Has)
    .def("Size", &OptionMap::Size)
    .def("GetType", &OptionMap::GetType)
    .def("ResetToDefault", &OptionMap::ResetToDefault)
    .def("IsValid", &OptionMap::IsValid)
    .def("Print", &OptionMap::Print)
    .def("Change", &OptionMap::ChangePy)
    .def("ChangeDict", &OptionMap::ChangePyDict);



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
    .def("Has", &CalcData::Has)
    .def("GetCopy", &CalcData::GetCopy<boost::python::object>)
//    .def("GetRef", &CalcData::GetRef<boost::python::object>, return_value_policy<copy_const_reference>()) 
    .def("Set", &CalcData::Set<boost::python::object>)
    .def("Erase", &CalcData::Erase)
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &, const std::string &)>(&CalcData::SetRef))
    .def("SetRef", static_cast<void(CalcData::*)(const CalcData &, const std::string &)>(&CalcData::SetRef));
}


} // close namespace export_python
} // close namespace datastore
} // close namespace bpmodule

