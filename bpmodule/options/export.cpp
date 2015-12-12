/*! \file
 *
 * \brief Python exports for options stuff
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/copy_const_reference.hpp>

#include "bpmodule/options/OptionMap.hpp"

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
    class_<OptionMap>("OptionMap", init<const std::string &, const boost::python::dict &, const boost::python::object &>())
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
    .def("ModuleKey", &OptionMap::ModuleKey, return_value_policy<copy_const_reference>())
    .def("Compare", &OptionMap::Compare)
    .def("CompareSelect", &OptionMap::CompareSelectPy)
    .def("HasIssues", &OptionMap::HasIssues);
}


} // close namespace export_python
} // close namespace options
} // close namespace bpmodule

