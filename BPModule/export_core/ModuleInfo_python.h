#ifndef MODULEINFO_PYTHON_H
#define MODULEINFO_PYTHON_H

#include "BPModule/core/ModuleInfo.h"

#include <boost/python.hpp>


namespace bpmodule {
namespace export_python {

ModuleInfo DictToModuleInfo(const boost::python::dict & dictionary);

boost::python::dict ModuleInfoToDict(const ModuleInfo & mi);


struct ModuleInfoConverter
{
    static PyObject* convert(const ModuleInfo & m)
    {
        boost::python::dict d = ModuleInfoToDict(m);
        return boost::python::incref(d.ptr());
    }
};



} // close namespace export_python
} // close namespace bpmodule

#endif
