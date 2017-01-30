/*! \file
 *
 * \brief Implementations for module smart pointers (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/modulemanager/ModuleIMPLHolder.hpp"
#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace detail {


PyModuleIMPLHolder::PyModuleIMPLHolder(const pybind11::object & mod)
    : mod_(mod)
{
    if(!mod)
        throw PulsarException("PyModuleIMPLHolder given a null object");
}

ModuleBase * PyModuleIMPLHolder::as_cpp_ptr(void) const
{
    return convert_to_cpp<ModuleBase *>(mod_);
}

pybind11::object PyModuleIMPLHolder::release_py_object(void)
{
    pybind11::object o(mod_);
    mod_.release();
    return o;
}



} // close namespace detail
} // close namespace pulsar
