/*! \file
 *
 * \brief Registers a UIDPoitner to python (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <string>
#include <unordered_map>

#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/exception/Exceptions.hpp"

using namespace bpmodule::exception;


namespace {
    std::unordered_map<std::string, std::function<pybind11::object(pybind11::object)>> creators_;
}


namespace bpmodule {
namespace datastore {

namespace detail {



void AddCreator_(const char * classname, std::function<pybind11::object(pybind11::object)> func)
{
    // will not overwrite if existing
    creators_.emplace(classname, func);
}
 

} // close namespace detail




pybind11::object MakeUIDPointerPy(const pybind11::object & obj)
{
    const std::string cn = python::GetPyClass(obj);
    if(!creators_.count(cn))
        throw PythonConvertException("No handler to create a UIDPointer", cn, "UIDPointer");

    return creators_.at(cn)(obj);
}





} // close namespace datastore
} // close namespace bpmodule


