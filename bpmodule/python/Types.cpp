/*! \file
 *
 * \brief Handling and detection of python types (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <stdexcept>

#include "bpmodule/python/Types.hpp"
#include "bpmodule/python/Pybind11.hpp"


namespace bpmodule {
namespace python {



PythonType StrToPythonType(const std::string & str)
{
    if(str == "bool")          return PythonType::Bool;
    if(str == "int")           return PythonType::Int;
    if(str == "float")         return PythonType::Float;
    if(str == "str")           return PythonType::String;
    if(str == "listbool")      return PythonType::ListBool;
    if(str == "listint")       return PythonType::ListInt;
    if(str == "listfloat")     return PythonType::ListFloat;
    if(str == "liststr")       return PythonType::ListString;
    if(str == "listempty")     return PythonType::ListEmpty;
    if(str == "listhetero")    return PythonType::ListHetero;
    if(str == "listunknown")   return PythonType::ListUnknown;
    if(str == "tuple")         return PythonType::Tuple;
    if(str == "dict")          return PythonType::Dict;
    if(str == "none")          return PythonType::None;

    return PythonType::Unknown;
}



const char * PythonTypeToStr(PythonType pytype)
{
    switch(pytype)
    {
        case PythonType::Bool:        return "bool";
        case PythonType::Int:         return "int";
        case PythonType::Float:       return "float";
        case PythonType::String:      return "str";
        case PythonType::ListBool:    return "listbool";
        case PythonType::ListInt:     return "listint";
        case PythonType::ListFloat:   return "listfloat";
        case PythonType::ListString:  return "liststr";
        case PythonType::ListEmpty:   return "listempty";
        case PythonType::ListHetero:  return "listhetero";
        case PythonType::ListUnknown: return "listunknown";
        case PythonType::Tuple:       return "tuple";
        case PythonType::Dict:        return "dict";
        case PythonType::None:        return "none";

        // Should only be thrown if we forget a PythonType above
        default:                      throw std::logic_error("Unknown PythonType to convert to string");
    }
}


std::string GetPyClass(pybind11::object obj)
{
    //! \todo should ever throw?
    pybind11::object cls = obj.attr("__class__");
    pybind11::object name = cls.attr("__name__");
    return name.cast<std::string>();
}


PythonType DeterminePyType(pybind11::object obj)
{
    try {
        if(obj.ptr() == nullptr)
            return PythonType::None;

        std::string cl = GetPyClass(obj);

        if(cl == "bool")      return PythonType::Bool;
        if(cl == "int")       return PythonType::Int;
        if(cl == "float")     return PythonType::Float;
        if(cl == "str")       return PythonType::String;
        if(cl == "tuple")     return PythonType::Tuple;
        if(cl == "dict")      return PythonType::Dict;
        if(cl == "NoneType")  return PythonType::None;

        if(cl == "list")
        {
            pybind11::list lst = obj.cast<pybind11::list>();

            size_t length = lst.size();
            if(length == 0)
                return PythonType::ListEmpty;

            // get type of first element
            std::string cl2 = GetPyClass(lst[0]);

            // check if this is a homogeneous container
            for(size_t i = 1; i < length; i++)
            {
                std::string cltmp = GetPyClass(lst[i]);
                if(cl2 != cltmp)
                    return PythonType::ListHetero;
            }

            if(cl2 == "bool")      return PythonType::ListBool;
            if(cl2 == "int")       return PythonType::ListInt;
            if(cl2 == "float")     return PythonType::ListFloat;
            if(cl2 == "str")       return PythonType::ListString;
            if(cl2 == "NoneType")  return PythonType::ListEmpty;

            return PythonType::ListUnknown;
        }
    }
    catch(...)
    {
        return PythonType::Unknown;
    }

    return PythonType::Unknown;
}


bool HasAttr(pybind11::object obj, const std::string & attr)
{
    return PyObject_HasAttrString(obj.ptr(), attr.c_str());
}

bool IsCallable(pybind11::object obj)
{
    return PyCallable_Check(obj.ptr());
}

bool HasCallableAttr(pybind11::object obj, const std::string & attr)
{
    return HasAttr(obj, attr) && IsCallable(obj.attr(attr.c_str()));
}


bool Equal(pybind11::object lhs, pybind11::object rhs)
{
    //! \todo check for error - res = -1
    int res = PyObject_RichCompareBool(lhs.ptr(), rhs.ptr(), Py_EQ);
    return (res == 1);
        
}


} // close namespace python
} // close namespace bpmodule

