#include <string>
#include <unordered_map>

#include "bpmodule/datastore/RegisterUIDPointer.hpp"
#include "bpmodule/exception/PythonConvertException.hpp"

using namespace bpmodule::exception;


namespace {
    std::unordered_map<std::string, std::function<pybind11::object(pybind11::object)>> creators_;
}


namespace bpmodule {
namespace datastore {

namespace detail {



void AddCreator_(const char * classname, std::function<pybind11::object(pybind11::object)> func)
{
    //! \todo - emplace? Throw if already registered?
    creators_[classname] = func;
}
 

} // close namespace detail




pybind11::object MakeUIDPointerPy(pybind11::object obj)
{
    const std::string cn = python_helper::GetPyClass2(obj);
    if(!creators_.count(cn))
        throw PythonConvertException("No handler to create a UIDPointer", cn, "UIDPointer");

    return creators_.at(cn)(obj);
}





} // close namespace datastore
} // close namespace bpmodule


