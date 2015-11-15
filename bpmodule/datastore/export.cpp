/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>

#include "bpmodule/datastore/CalcData.hpp"
#include "bpmodule/datastore/CacheData.hpp"


using namespace boost::python;


namespace bpmodule {
namespace datastore {
namespace export_python {

BOOST_PYTHON_MODULE(datastore)
{
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

