#include "BPModule/datastore/PropertyMap.hpp"
#include "BPModule/datastore/OptionMap.hpp"
#include "BPModule/datastore/CalcData.hpp"

#include <boost/python.hpp>

using namespace boost::python;


namespace bpmodule {
namespace datastore {
namespace export_python {


BOOST_PYTHON_MODULE(bpmodule_datastore)
{
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

