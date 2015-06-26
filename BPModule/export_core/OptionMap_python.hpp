#ifndef OPTIONMAP_PYTHON_H
#define OPTIONMAP_PYTHON_H

#include "BPModule/core/OptionMap.hpp"

#include <boost/python.hpp>

namespace bpmodule {

namespace export_python {


boost::python::list OptionMapToList(const OptionMap & op);


void OptionMap_Set_Helper(OptionMap * op, const std::string & key,
                          const boost::python::object & value,
                          const std::string & help);


boost::python::object OptionMap_Get_Helper(const OptionMap * op, const std::string & key);


OptionMap ListToOptionMap(const boost::python::list & olist);


// Converter for OptionMap
struct OptionMapConverter
{
    static PyObject* convert(const OptionMap & op)
    {
        boost::python::list lst = OptionMapToList(op);
        return boost::python::incref(lst.ptr());
    }
};


} // close namespace export_python
} // close namespace bpmodule

#endif
