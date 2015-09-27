/*! \file
 *
 * \brief Conversion between python and C++ (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/python_helper/Convert.hpp"


namespace bpmodule {
namespace python_helper {



PythonType StrToPythonType(const std::string & str) noexcept
{
    if(str == "bool")
        return PythonType::Bool;
    if(str == "int")
        return PythonType::Int;
    if(str == "float")
        return PythonType::Float;
    if(str == "str")
        return PythonType::String;

    if(str == "listbool")
        return PythonType::ListBool;
    if(str == "listint")
        return PythonType::ListInt;
    if(str == "listfloat")
        return PythonType::ListFloat;
    if(str == "liststr")
        return PythonType::ListString;

    if(str == "listempty")
        return PythonType::ListEmpty;
    if(str == "listhetero")
        return PythonType::ListHetero;
    if(str == "listunknown")
        return PythonType::ListUnknown;

    if(str == "tuple")
        return PythonType::Tuple;

    if(str == "dict")
        return PythonType::Dict;

    if(str == "none")
        return PythonType::None;

    return PythonType::Unknown;
}



const char * PythonTypeToStr(PythonType pytype) noexcept
{
    switch(pytype)
    {
        case PythonType::Bool:
            return "bool";

        case PythonType::Int:
            return "int";

        case PythonType::Float:
            return "float";

        case PythonType::String:
            return "str";

        case PythonType::ListBool:
            return "listbool";

        case PythonType::ListInt:
            return "listint";

        case PythonType::ListFloat:
            return "listfloat";

        case PythonType::ListString:
            return "liststr";

        case PythonType::ListEmpty:
            return "listempty";

        case PythonType::ListHetero:
            return "listhetero";

        case PythonType::ListUnknown:
            return "listunknown";

        case PythonType::Tuple:
            return "tuple";

        case PythonType::Dict:
            return "dict";

        case PythonType::None:
            return "none";

        default:
            return "unknown";
    }
}




PythonType DetermineType(const boost::python::object & obj)
{
    try {
        std::string cl = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));

        if(cl == "bool")
            return PythonType::Bool;

        if(cl == "int")
            return PythonType::Int;

        if(cl == "float")
            return PythonType::Float;

        if(cl == "str")
            return PythonType::String;


        if(cl == "NoneType")
            return PythonType::None;


        if(cl == "list")
        {
            boost::python::list lst = boost::python::extract<boost::python::list>(obj);

            int length = boost::python::extract<int>(lst.attr("__len__")());
            if(length <= 0)
                return PythonType::ListEmpty;

            // get type of first element
            std::string cl2 = boost::python::extract<std::string>(lst[0].attr("__class__").attr("__name__"));

            // check if this is a homogeneous container
            for(int i = 1; i < length; i++)
            {
                std::string cltmp = boost::python::extract<std::string>(lst[i].attr("__class__").attr("__name__"));
                if(cl2 != cltmp)
                    return PythonType::ListHetero;
            }

            if(cl2 == "bool")
                return PythonType::ListBool;

            if(cl2 == "int")
                return PythonType::ListInt;

            if(cl2 == "float")
                return PythonType::ListFloat;

            if(cl2 == "str")
                return PythonType::ListString;

            if(cl2 == "NoneType")
                return PythonType::ListEmpty;

            return PythonType::ListUnknown;
        }

        if(cl == "tuple")
            return PythonType::Tuple;

        if(cl == "dict")
            return PythonType::Dict;

    }
    catch(...)
    {
        return PythonType::Unknown;
    }

    return PythonType::Unknown;
}




} // close namespace python_helper
} // close namespace bpmodule

