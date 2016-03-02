/*! \file
 *
 * \brief Implementations for module smart pointers (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/modulemanager/ModuleIMPLHolder.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulemanager {
namespace detail {


PyModuleIMPLHolder::PyModuleIMPLHolder(const pybind11::object & mod)
    : mod_(mod)
{ 
    using namespace bpmodule::exception;

    if(!mod)
        throw GeneralException("PyModuleIMPLHolder given a null object");
}

modulebase::ModuleBase * PyModuleIMPLHolder::CppPtr(void) const
{
    return python::ConvertToCpp<modulebase::ModuleBase *>(mod_);
}

pybind11::object PyModuleIMPLHolder::PythonObject(void) const
{
    return mod_; 
}



} // close namespace detail
} // close namespace modulemanager
} // close namespace bpmodule
