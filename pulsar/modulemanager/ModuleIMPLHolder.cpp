/*! \file
 *
 * \brief Implementations for module smart pointers (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/modulemanager/ModuleIMPLHolder.hpp"
#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulemanager {
namespace detail {


PyModuleIMPLHolder::PyModuleIMPLHolder(const pybind11::object & mod)
    : mod_(mod)
{
    

    if(!mod)
        throw GeneralException("PyModuleIMPLHolder given a null object");
}

modulebase::ModuleBase * PyModuleIMPLHolder::CppPtr(void) const
{
    return python::convert_to_cpp<modulebase::ModuleBase *>(mod_);
}

pybind11::object PyModuleIMPLHolder::PythonObject(void) const
{
    return mod_;
}



} // close namespace detail
} // close namespace modulemanager
} // close namespace pulsar
