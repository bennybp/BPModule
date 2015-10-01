/*! \file
 *
 * \brief Python exports for data storage
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/options/OptionMap.hpp"

#include <boost/python.hpp>

using namespace boost::python;


namespace bpmodule {
namespace options {
namespace export_python {

BOOST_PYTHON_MODULE(options)
{
    /////////////////////////
    // OptionMap
    /////////////////////////
    // OptionMap always returns copies, so this should be safe
    class_<OptionMap>("OptionMap", init<const boost::python::dict &>())
    .def("MaxInt", &OptionMap::MaxInt)
    .def("MinInt", &OptionMap::MinInt)
    .def("MaxFloat", &OptionMap::MaxFloat)
    .def("MinFloat", &OptionMap::MinFloat)
    .def("Get", &OptionMap::GetPy)
    .def("Has", &OptionMap::Has)
    .def("HasKey", &OptionMap::HasKey)
    .def("Size", &OptionMap::Size)
    .def("ResetToDefault", &OptionMap::ResetToDefault)
    .def("IsValid", &OptionMap::IsValid)
    .def("Print", &OptionMap::Print)
    .def("Change", &OptionMap::ChangePy)
    .def("ChangeDict", &OptionMap::ChangePyDict);
}


} // close namespace export_python
} // close namespace options
} // close namespace bpmodule

